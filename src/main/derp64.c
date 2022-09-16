#include <ultra64.h>

#include "derp64.h"
#include "pumpkin.h"
#include "cube.h"

//Functions defines? I dont remember the term
static void idle(void *arg);
static void run(void *arg);
static void initGame(void);

u64	bootStack[STACKSIZE/sizeof(u64)];

//Stacks and threads
static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/sizeof(u64)];

OSPiHandle	*handler;

/* this number (the depth of the message queue) needs to be equal
 * to the maximum number of possible overlapping PI requests.
 */
#define NUM_PI_MSGS     8

static OSMesg PiMessages[NUM_PI_MSGS];
static OSMesgQueue PiMessageQ;


OSMesgQueue	dmaMessageQ, rdpMessageQ, retraceMessageQ;
OSMesg		dmaMessageBuf, rdpMessageBuf, retraceMessageBuf;

// OSTask tlist =
// {
//     M_GFXTASK,			/* task type */
//     OS_TASK_DP_WAIT,		/* task flags */
//     NULL,			/* boot ucode pointer (fill in later) */
//     0,				/* boot ucode size (fill in later) */
//     gspF3DEX2_xbusTextStart,			/* task ucode pointer (fill in later) */
//     SP_UCODE_SIZE,		/* task ucode size */
//     gspF3DEX2_xbusDataStart,			/* task ucode data pointer (fill in later) */
//     SP_UCODE_DATA_SIZE,		/* task ucode data size */
//     &dram_stack[0],		/* task dram stack pointer */
//     SP_DRAM_STACK_SIZE8,	/* task dram stack size */
//     &rdp_output[0],		/* task fifo buffer start ptr */
//     &rdp_output[0]+RDP_OUTPUT_LEN, /* task fifo buffer end ptr */
//     NULL,			/* task data pointer (fill in later) */
//     0,				/* task data size (fill in later) */
//     NULL,			/* task yield buffer ptr (not used here) */
//     0				/* task yield buffer size (not used here) */
// };

//Graphics Stuff
GraphicsInfo gInfo[2];

static int      draw_buffer = 0;

void boot() 
{
    osInitialize();

    handler = osCartRomInit();

    osCreateThread(&idleThread, 1, idle, (void*) 0, idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);
}

static void idle(void *arg)
{
    /* Initialize video */
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    
    /*
     * Start PI Mgr for access to cartridge
     */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		      NUM_PI_MSGS);

    /*
     * Create main thread
     */
    osCreateThread(&mainThread, 3, run, NULL,
		   mainThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&mainThread);

    /*
     * Become the idle thread
     */
    osSetThreadPri(0, 0);

    for(;;);
}

void romCopy(const char* src, const char* dest, const int len) 
{
    OSIoMesg dmaIOMessageBuf;
    OSMesg dummyMesg;

     /*
     * Always invalidate cache before dma'ing data into the buffer.
     * This is to prevent a flush of the cache in the future from 
     * potentially trashing some data that has just been dma'ed in.
     * Since you don't care if old data makes it from cache out to 
     * memory, you can use the cheaper osInvalDCache() instead of one
     * of the writeback commands
     */
    osInvalDCache((void *)dest, (s32) len);

    dmaIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIOMessageBuf.hdr.retQueue = &dmaMessageQ;
    dmaIOMessageBuf.dramAddr     = (void*)dest;
    dmaIOMessageBuf.devAddr      = (u32)src;
    dmaIOMessageBuf.size         = (u32)len;

    // dmaIOMessageBuf.dramAddr     = (staticSegment);
    // dmaIOMessageBuf.devAddr      = (u32)_staticSegmentRomStart;
    // dmaIOMessageBuf.size         = (u32)_staticSegmentRomEnd-(u32)_staticSegmentRomStart;

    osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);

    /*
     * Wait for DMA to finish
     */
    (void)osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);
}

static void run(void *arg) 
{
    OSTask *tlistp;
    
    initGame();

    initGraphics();
    
    while (1) 
    {
        render(&gInfo[draw_buffer]);

        draw_buffer ^= 1;
    }
}

static void initGame(void) 
{
    /*
     * Setup the message queues
     */
    osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
    
    osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
    osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);
    
    osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
    osViSetEvent(&retraceMessageQ, NULL, 1);
}