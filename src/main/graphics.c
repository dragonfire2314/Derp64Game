#include <ultra64.h>

#include "derp64.h"
#include "graphics.h"

char *staticSegment = 0;

void initGraphics(void) 
{
    /*
     * Stick the static segment right after the code/data segment
     */
    staticSegment = _codeSegmentBssEnd;
    romCopy(_staticSegmentRomStart, staticSegment, _staticSegmentRomEnd-_staticSegmentRomStart);

    gInfo[0].cfb = cfb_16_a;
    gInfo[1].cfb = cfb_16_b;
}

int spriteX = 0;
int spriteY = 0;
int spriteXdir = 1;
int spriteYdir = 1;

void render(GraphicsInfo *i) 
{
    spriteX += 1 * spriteXdir;
    spriteY += 1 * spriteYdir;

    if (spriteX > SCREEN_WIDTH-32)
        spriteXdir *= -1;
    if (spriteX < 0)
        spriteXdir *= -1;
    if (spriteY > SCREEN_HEIGHT-32)
        spriteYdir *= -1;
    if (spriteY < 0)
        spriteYdir *= -1;


    Gfx *glistp = i->dp.glist;
    OSTask *t;
    Dynamic *dynamicp = &i->dp;

    /*
        CLEAR FRAME AND SETUP NEXT FRAME
    */
    gSPSegment(glistp++, 0, 0x0);
    gSPSegment(glistp++, STATIC_SEGMENT, OS_K0_TO_PHYSICAL(staticSegment));
    gSPSegment(glistp++, CFB_SEGMENT, OS_K0_TO_PHYSICAL(i->cfb));

    //Setup rdp and rsp
    gSPDisplayList(glistp++, rdpinit_dl);
    gSPDisplayList(glistp++, rspinit_dl);
    //Clear the frame buffer
    gSPDisplayList(glistp++, clearframebuffer_dl);
    /*
        END ---CLEAR FRAME AND SETUP NEXT FRAME
    */
    

    /*
        RENDER GAME
    */
    gSPDisplayList(glistp++, sprite_dl);
    /*
        END ---RENDER GAME
    */

    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);

    /* Flush the dynamic segment */
    osWritebackDCache(&i->dp, (s32)glistp - (s32)&i->dp);

    // Setup task
    t = &i->task;

    t->t.type = M_GFXTASK;
    t->t.flags = OS_TASK_DP_WAIT;

    t->t.ucode_boot = (u64*)rspbootTextStart;
    t->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

    t->t.ucode = gspF3DEX2_xbusTextStart;
    t->t.ucode_size = SP_UCODE_SIZE;

    t->t.ucode_data = gspF3DEX2_xbusDataStart;
    t->t.ucode_data_size = SP_UCODE_DATA_SIZE;

    t->t.dram_stack = &dram_stack[0];
    t->t.dram_stack_size = SP_DRAM_STACK_SIZE8;

    t->t.output_buff = &rdp_output[0];
    t->t.output_buff_size = &rdp_output[0]+RDP_OUTPUT_LEN;

    t->t.data_ptr = (u64*)dynamicp->glist;
    t->t.data_size = sizeof(*dynamicp->glist) * (glistp - dynamicp->glist);

    t->t.yield_data_ptr = NULL;
    t->t.yield_data_size = 0;

    //Start the task and wait for a response
    osSpTaskStart(t);
    osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

    // Swap the buffers
    osViSwapBuffer(i->cfb);

    /* Make sure there isn't an old retrace in queue 
    * (assumes queue has a depth of 1) 
    */
    if (MQ_IS_FULL(&retraceMessageQ))
        (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
    
    /* Wait for Vertical retrace to finish swap buffers */
    (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
}