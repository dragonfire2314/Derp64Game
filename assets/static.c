#include <ultra64.h>
#include "derp64.h"

static Vp vp = {
	SCREEN_WIDTH*2, SCREEN_HEIGHT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WIDTH*2, SCREEN_HEIGHT*2, G_MAXZ/2, 0,	/* translate */
};

/* initialize the RSP state: */
Gfx rspinit_dl[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

/* initialize the RDP state: */
Gfx rdpinit_dl[] = {
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipelineMode(G_PM_1PRIMITIVE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};


/* clear the color frame buffer: */

Gfx clearframebuffer_dl[] = {
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, rsp_cfb),
    gsDPSetFillColor(GPACK_RGBA5551(64,64,255,1) << 16 | 
		     GPACK_RGBA5551(64,64,255,1)),
    gsDPFillRectangle(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Gfx sprite_dl[] = {
    gsDPPipeSync(),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetCycleType(G_CYC_COPY),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPTexture(0x2000, 0x2000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPLoadTextureBlock(pumpkin, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32,
                         0, G_TX_NOMIRROR, G_TX_NOMIRROR, 0, 0,
                         G_TX_NOLOD, G_TX_NOLOD),
    gsSPTextureRectangle(40 << 2, 10 << 2, (40 + 32 - 1) << 2,
                         (10 + 32 - 1) << 2, 0, 0, 0, 4 << 10,
                         1 << 10),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};