#ifndef DERP_64_H
#define DERP_64_H

#define	SCREEN_HEIGHT	240
#define	SCREEN_WIDTH	320

#define	STACKSIZE	0x2000

#define	STATIC_SEGMENT		1
#define	CFB_SEGMENT		2

#define	SP_DRAM_STACK_SIZE8	(1024)
#define	SP_DRAM_STACK_SIZE64	(SP_DRAM_STACK_SIZE8 >> 3)


#ifdef _LANGUAGE_C

#include "graphics.h"

extern char _codeSegmentBssEnd[];
extern char _staticSegmentRomStart[], _staticSegmentRomEnd[];

//Function defines
// void renderer();
void initGraphics(void);
void render(GraphicsInfo *i);
void romCopy(const char* src, const char* dest, const int len);

extern u64 dram_stack[];	/* used for matrix stack */
extern u64 rdp_output[];	/* RSP writes back RDP data */
#define RDP_OUTPUT_LEN (4096*16)

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx	clearframebuffer_dl[];

extern Gfx shadetri_dl[];

extern Gfx sprite_dl[];
extern Gfx cube_dl[];

extern Vtx cube_vertex[];
extern u16 pumpkin[];

extern OSMesgQueue	dmaMessageQ, rdpMessageQ, retraceMessageQ;
extern OSMesg		dmaMessageBuf, rdpMessageBuf, retraceMessageBuf;

/* RSP address for the color frame buffer */

extern u16	rsp_cfb[];

#endif	/* _LANGUAGE_C */

#endif //DERP_64_H