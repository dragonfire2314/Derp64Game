

#define	SCREEN_HEIGHT	240
#define	SCREEN_WIDTH	320

#define	STACKSIZE	0x2000

#define	STATIC_SEGMENT		1
#define	CFB_SEGMENT		2

#define	SP_DRAM_STACK_SIZE8	(1024)
#define	SP_DRAM_STACK_SIZE64	(SP_DRAM_STACK_SIZE8 >> 3)

//Function defines
// void renderer();


#ifdef _LANGUAGE_C

typedef struct {
	Mtx	projection;
	Mtx	modeling;
	Mtx	viewing;
    Mtx identity;
} Dynamic;
extern Dynamic dynamic;

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

/* CPU addresses for the color frame buffer */

extern u16	cfb[][SCREEN_WIDTH*SCREEN_HEIGHT];

/* RSP address for the color frame buffer */

extern u16	rsp_cfb[];

#endif	/* _LANGUAGE_C */