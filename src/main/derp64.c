#include <ultra64.h>

#include "derp64.h"

//Functions defines? I dont remember the term
static void idle(void *arg);

//Boot stack
u64	bootStack[STACKSIZE/sizeof(u64)];

//Stacks and threads
static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/sizeof(u64)];

OSPiHandle	*handler;

void boot() 
{
    osInitialize();

    handler = osCartRomInit();

    osCreateThread(&idleThread, 1, idle, (void*) 0, idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);
}

static void idle(void *arg)
{
    //Write to the frame buffer?
    for(int i = 0; i < 76000; i++)
    {
        cfb[0][i*2 + 1000] = 0x7FFF;
        cfb[1][i*2 + 1000] = 0x7FFF;
    }

    for(;;);
}