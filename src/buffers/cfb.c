/*
 * CPU view of the actual frame buffer memory.
 */

#include <ultra64.h>
#include "derp64.h"

u16 cfb_16_a[SCREEN_WIDTH*SCREEN_HEIGHT];
u16 cfb_16_b[SCREEN_WIDTH*SCREEN_HEIGHT];
