#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ultra64.h>

typedef struct {
	Mtx	projection;
	Mtx	viewing;
	Gfx	glist[512];	/* buffer to hold display list */
} Dynamic;

typedef struct {
    OSTask task;
    Dynamic dp;
    u16 *cfb;
} GraphicsInfo;

extern GraphicsInfo gInfo[];

//Buffers
// extern u16	cfb[][SCREEN_WIDTH*SCREEN_HEIGHT];
extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];
// extern unsigned short zbuffer[];

//PLayer?
extern int spriteX;
extern int spriteY;

#endif //GRAPHICS_H