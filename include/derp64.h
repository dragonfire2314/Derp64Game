

#define	SCREEN_HT	240
#define	SCREEN_WD	320

#define	STACKSIZE	0x2000


#ifdef _LANGUAGE_C

/* CPU addresses for the color frame buffer */

extern u16	cfb[][SCREEN_WD*SCREEN_HT];

/* RSP address for the color frame buffer */

extern u16	rsp_cfb[];

#endif	/* _LANGUAGE_C */