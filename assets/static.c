#include <ultra64.h>
#include "derp64.h"

// // Viewport scaling parameters.
// static Vp vp = {{
//     .vscale = {SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, G_MAXZ / 2, 0},
//     .vtrans = {SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, G_MAXZ / 2, 0},
// }};

// // Initialize the RSP.
// Gfx rspinit_dl[] = {
//     gsSPViewport(&vp),
//     gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
//                           G_FOG | G_LIGHTING | G_TEXTURE_GEN |
//                           G_TEXTURE_GEN_LINEAR | G_LOD),
//     gsSPTexture(0, 0, 0, 0, G_OFF),
//     gsSPEndDisplayList(),
// };

// // Initialize the RDP.
// Gfx rdpinit_dl[] = { 
//     gsDPSetCycleType(G_CYC_1CYCLE),
//     gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH,
//                    SCREEN_HEIGHT),
//     gsDPSetCombineKey(G_CK_NONE),
//     gsDPSetAlphaCompare(G_AC_NONE),
//     gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
//     gsDPSetColorDither(G_CD_DISABLE),
//     gsDPPipeSync(),
//     gsSPEndDisplayList(),
// };

// // Clear the color framebuffer.
// Gfx clearframebuffer_dl[] = {
//     gsDPSetCycleType(G_CYC_FILL),
//     gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, rsp_cfb),
//     gsDPPipeSync(),
//     gsDPSetFillColor(GPACK_RGBA5551(255,0,0,1)<<16 | GPACK_RGBA5551(255,0,0,1)),
//     gsDPFillRectangle(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1),
//     gsSPEndDisplayList(),
// };