/*****************************************************************************/
/*                                                                           */
/* TIFFSDK.C                                                                 */
/*                                                                           */
/* Version: 1.0                                                              */
/*                                                                           */
/* This file is the 'magic' behind the TIFF SDK.  So long as this file is    */
/* included in your project's build, your application's API calls will be    */
/* automatically linked at runtime, and your main() function called.         */
/*                                                                           */
/* Note that this file currently contains a selection of commonly-used API   */
/* calls.  If you want to use an API function that isn't already in this 		 */
/* file, then you will need to add those functions to this file, as follows: */
/*                                                                           */
/*  * Check that the API call you want to use is supported by the SDK.       */
/*    (The list of supported calls is given in the file 'functions.txt').    */
/*                                                                           */
/*  * Add a call to resolveSyscall for the function you want to use, in      */
/*    the init() function below, following the pattern shown.  The second    */
/*    parameter is the NID code for the function, obtained from 						 */
/* 		functions.txt. 																												 */
/*                                                                           */
/*  * Add a stub function to the set just before the init() function,        */
/*    following the pattern shown.                                           */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* DO NOT CHANGE THIS SECTION                                                */
/*                                                                           */
/*****************************************************************************/
#include <pspuser.h>
#include <pspctrl.h>

typedef unsigned long (*PFN_RESOLVEFUNC)(unsigned long);
void (*tiffsdkKernelDcacheWritebackAll)(void);
#define FUNC_TO_JMP(A)  (((((unsigned long)((void*)(A))) & 0x0FFFFFFF) >> 2) | 0x08000000L)

PFN_RESOLVEFUNC findFunctionByNid;

unsigned long failedNID;

/*****************************************************************************/
/* Set by the linker to be the first instruction                             */
/*****************************************************************************/
void _start(unsigned long, unsigned long *) __attribute__ ((section (".text.start")));
void _start(unsigned long arglen, unsigned long *argp)
{	findFunctionByNid = (PFN_RESOLVEFUNC*)argp[1];	init();
	main(argp[0]);
}

/*****************************************************************************/
/* Flash screen 10 times                                                     */
/*****************************************************************************/
void flashScreen(int cola, int colb)
{
  {
    int i;

    for (i=0; i<10; i++)
    {
      {
        long *lptr = 0x44000000;

        while (lptr < 0x44220000)
        {
          *lptr++ = cola;
        }

        lptr = 0x44000000;

        while (lptr < 0x44220000)
        {
          *lptr++ = colb;
        }
      }
    }
  }
}

/*****************************************************************************/
/* Retrieves the address to jump to for a given NID, then writes the 				 */
/* necessary opcodes into the stub function.                               	 */
/*****************************************************************************/
resolveSyscall(void *funcaddr, unsigned long nid)
{
	unsigned long realFunc = findFunctionByNid(nid);

	if (!realFunc)
	{
    /*************************************************************************/
		/* We got an unresolved function.  We can't guarantee that we currently  */
    /* have enough functions resolved to display anything meaningful, so we  */
    /* just flash the screen as a warning.                                   */
		/*************************************************************************/
		flashScreen(0, 0x000000FFL);
		failedNID = nid;
	}

	if(nid==0x79d1c3faL) tiffsdkKernelDcacheWritebackAll = (void*)realFunc;
	
	*((unsigned long*)funcaddr) = FUNC_TO_JMP(realFunc);
	*(((unsigned long*)funcaddr) + 1) = 0;
}

/*****************************************************************************/
/*                                                                           */
/* END OF 'DO NOT CHANGE THIS SECTION'                                       */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Function stubs start here.  All SCE API functions that are used must      */
/* have a corresponding function stub.                                       */
/*****************************************************************************/
int sceCtrlGetSamplingMode(int *pmode)
{
  asm ("syscall 0x0000;");
};

int sceCtrlPeekBufferPositive(SceCtrlData *pad_data, int count)
{
  asm ("syscall 0x0000;");
};

int sceCtrlReadBufferPositive(SceCtrlData *pad_data, int count)
{
  asm ("syscall 0x0000;");
};

int sceCtrlSetSamplingMode(int mode)
{
  asm ("syscall 0x0000;");
};

void sceDisplaySetFrameBuf(void *topaddr, int bufferwidth, int pixelformat, int sync)
{
  asm ("syscall 0x0000;");
};

int sceDisplaySetMode(int mode, int width, int height)
{
  asm ("syscall 0x0000;");
};

int sceDisplayWaitVblankStart(void)
{
  asm ("syscall 0x0000;");
};

int sceIoClose(SceUID fd)
{
  asm ("syscall 0x0000;");
};

int sceIoDclose(SceUID fd)
{
  asm ("syscall 0x0000;");
};

SceUID sceIoDopen(const char *dirname)
{
  asm ("syscall 0x0000;");
};

int sceIoDread(SceUID fd, SceIoDirent *dir)
{
  asm ("syscall 0x0000;");
};

SceOff sceIoLseek(SceUID fd, SceOff offset, int whence)
{
  asm ("syscall 0x0000;");
};

int sceIoMkdir(const char *dir, SceMode mode)
{
  asm ("syscall 0x0000;");
};

SceUID sceIoOpen(const char *file, int flags, SceMode mode)
{
  asm ("syscall 0x0000;");
};

int sceIoRead(SceUID fd, void *data, SceSize size)
{
  asm ("syscall 0x0000;");
};

int sceIoRemove(const char *file)
{
  asm ("syscall 0x0000;");
};

int sceIoRename(const char *oldname, const char *newname)
{
  asm ("syscall 0x0000;");
};

int sceIoRmdir(const char *path)
{
  asm ("syscall 0x0000;");
};

int sceIoWrite(SceUID fd, const void *data, SceSize size)
{
  asm ("syscall 0x0000;");
};

int sceKernelCancelEventFlag(SceUID evid)
{
  asm ("syscall 0x0000;");
};

int sceKernelCancelSema(SceUID semaid)
{
  asm ("syscall 0x0000;");
};

int sceKernelClearEventFlag(SceUID evid, u32 bits)
{
  asm ("syscall 0x0000;");
};

int sceKernelCreateCallback(const char *name, SceKernelCallbackFunction func, void *arg)
{
  asm ("syscall 0x0000;");
};

SceUID sceKernelCreateEventFlag(const char *name, int attr, int bits, SceKernelEventFlagOptParam *opt)
{
  asm ("syscall 0x0000;");
};

SceUID sceKernelCreateSema(const char *name, SceUInt attr, int initVal, int maxVal, SceKernelSemaOptParam *option)
{
  asm ("syscall 0x0000;");
};

SceUID sceKernelCreateThread(const char *name, SceKernelThreadEntry entry, int initPriority,
                             int stackSize, SceUInt attr, SceKernelThreadOptParam *option)
{
  asm ("syscall 0x0000;");
};

void sceKernelDcacheWritebackAll(void)
{
  asm ("syscall 0x0000;");
};

void sceKernelDcacheWritebackInvalidateRange(const void *p, unsigned int size)
{
  asm ("syscall 0x0000;");
};

void sceKernelDcacheWritebackRange(const void *p, unsigned int size)
{
  asm ("syscall 0x0000;");
};

void sceKernelDelayThread(SceUInt delay)
{
  asm ("syscall 0x0000;");
};

int sceKernelDeleteCallback(int cbid)
{
  asm ("syscall 0x0000;");
};

int sceKernelDeleteEventFlag(int evid)
{
  asm ("syscall 0x0000;");
};

int sceKernelDeleteFpl(SceUID uid)
{
  asm ("syscall 0x0000;");
};

int sceKernelDeleteSema(SceUID semaid)
{
  asm ("syscall 0x0000;");
};

int sceKernelDeleteThread(SceUID thid)
{
  asm ("syscall 0x0000;");
};

int sceKernelDeleteVpl(SceUID uid)
{
  asm ("syscall 0x0000;");
};

int sceKernelExitDeleteThread(int status)
{
  asm ("syscall 0x0000;");
};

int sceKernelExitThread(int status)
{
  asm ("syscall 0x0000;");
};

int sceKernelFreeFpl(SceUID uid, void *data)
{
  asm ("syscall 0x0000;");
};

int sceKernelFreeVpl(SceUID uid, void *data)
{
  asm ("syscall 0x0000;");
};

int sceKernelGetThreadId(void)
{
  asm ("syscall 0x0000;");
};

int sceKernelPollEventFlag(int evid, u32 bits, u32 wait, u32 *outBits)
{
  asm ("syscall 0x0000;");
};

int sceKernelPollSema(SceUID semaid, int signal)
{
  asm ("syscall 0x0000;");
};

sceKernelPowerTick()
{
  asm ("syscall 0x0000;");
};

int sceKernelSetEventFlag(SceUID evid, u32 bits)
{
  asm ("syscall 0x0000;");
};

int sceKernelSignalSema(SceUID semaid, int signal)
{
  asm ("syscall 0x0000;");
};

int sceKernelStartThread(SceUID thid, SceSize arglen, void *argp)
{
  asm ("syscall 0x0000;");
};

int sceKernelTerminateDeleteThread(SceUID thid)
{
  asm ("syscall 0x0000;");
};

/*int sceKernelWaitEventFlag(int evid, u32 bits, u32 wait, u32 *outBits, SceUInt *timeout)
{
  asm ("syscall 0x0000;");
};*/

int sceKernelWaitSema(SceUID semaid, int signal, SceUInt *timeout)
{
  asm ("syscall 0x0000;");
};

int sceKernelWaitThreadEnd(SceUID thid, SceUInt *timeout)
{
  asm ("syscall 0x0000;");
};

int sceRtcGetCurrentTick(u64 *tick)
{
  asm ("syscall 0x0000;");
};

int vshCtrlReadBufferPositive(SceCtrlData *pad_data, int count)
{
  asm ("syscall 0x0000;");
};

void vshKernelExitVSHVSH(void *unk_struct)
{
   asm ("syscall 0x0000;");
};
int vshKernelLoadExecVSHMs1(const char *file, struct SceKernelLoadExecVSHParam *param)
{
   asm ("syscall 0x0000;");
};
int sceUmdActivate(int unit, const char *drive)
{
   asm ("syscall 0x0000;");
};


int rand()
{
	asm ("syscall 0x0000;");
}

/**********GU/GUM************/
int sinf()
{
	asm ("syscall 0x0000;");
}
int cosf()
{
	asm ("syscall 0x0000;");
}
int sceGuAlphaFunc(int func, int value, int mask)
{
	asm ("syscall 0x0000;");
}
int sceGuAmbient(unsigned int color)
{
	asm ("syscall 0x0000;");
}
int sceGuBlendFunc(int op, int src, int dest, unsigned int srcfix, unsigned int destfix)
{
	asm ("syscall 0x0000;");
}
int sceGuCallList(const void* list)
{
	asm ("syscall 0x0000;");
}
int sceGuCallMode(int mode)
{
	asm ("syscall 0x0000;");
}
int sceGuClear(int flags)
{
	asm ("syscall 0x0000;");
}
int sceGuClearColor(unsigned int color)
{
	asm ("syscall 0x0000;");
}
int sceGuClearDepth(unsigned int depth)
{
	asm ("syscall 0x0000;");
}
int sceGuClearStencil(unsigned int stencil)
{
	asm ("syscall 0x0000;");
}
int sceGuClutLoad(int num_blocks, const void* cbp)
{
	asm ("syscall 0x0000;");
}
int sceGuClutMode(unsigned int cpsm, unsigned int a1, unsigned int a2, unsigned int a3)
{
	asm ("syscall 0x0000;");
}
int sceGuColor(unsigned int color)
{
	asm ("syscall 0x0000;");
}
int sceGuCopyImage(int psm, int sx, int sy, int width, int height, int srcw, void* src, int dx, int dy, int destw, void* dest)
{
	asm ("syscall 0x0000;");
}
int sceGuInit(void)
{
	asm ("syscall 0x0000;");
}
int sceGuStart(int cid, void* list)
{
	asm ("syscall 0x0000;");
}
int sceGuDrawBuffer(int psm, void* fbp, int fbw)
{
	asm ("syscall 0x0000;");
}
int sceGuDispBuffer(int width, int height, void* dispbp, int dispbw)
{
	asm ("syscall 0x0000;");
}
int sceGuDepthBuffer(void* zbp, int zbw)
{
	asm ("syscall 0x0000;");
}
int sceGuEnable(int state)
{
	asm ("syscall 0x0000;");
}
int sceGuFinish(void)
{
	asm ("syscall 0x0000;");
}
int sceGuOffset(unsigned int x, unsigned int y)
{
	asm ("syscall 0x0000;");
}
int sceGuViewport(int cx, int cy, int width, int height)
{
	asm ("syscall 0x0000;");
}
int sceGuDepthRange(int near, int far)
{
	asm ("syscall 0x0000;");
}
int sceGuScissor(int x, int y, int w, int h)
{
	asm ("syscall 0x0000;");
}
int sceGuDepthFunc(int function)
{
	asm ("syscall 0x0000;");
}
int sceGuFrontFace(int order)
{
	asm ("syscall 0x0000;");
}
int sceGuShadeModel(int mode)
{
	asm ("syscall 0x0000;");
}
int sceGuLight(int light, int type, int components, const ScePspFVector3* position)
{
	asm ("syscall 0x0000;");
}
int sceGuLightColor(int light, int component, unsigned int color)
{
	asm ("syscall 0x0000;");
}
int sceGuSync(int mode, int a1)
{
	asm ("syscall 0x0000;");
}
int sceGuDisplay(int state)
{
	asm ("syscall 0x0000;");
}
int sceGumMatrixMode(int mode)
{
	asm ("syscall 0x0000;");
}
int sceGumLoadIdentity(void)
{
	asm ("syscall 0x0000;");
}
int sceGumPerspective(float fovy, float aspect, float near, float far)
{
	asm ("syscall 0x0000;");
}
int sceGumTranslate(const ScePspFVector3* v)
{
	asm ("syscall 0x0000;");
}
int sceGumDrawArray(int prim, int vtype, int count, const void* indices, const void* vertices)
{
	asm ("syscall 0x0000;");
}
int sceGuSwapBuffers(void)
{
	asm ("syscall 0x0000;");
}
int sceGuTexMode(int tpsm, int maxmips, int a2, int swizzle)
{
	asm ("syscall 0x0000;");
}
int sceGuTexFunc(int tfx, int tcc)
{
	asm ("syscall 0x0000;");
}
int sceGuTexFilter(int min, int mag)
{
	asm ("syscall 0x0000;");
}

/*****************************************************************************/
/* Patches everything up ready for main() to execute.                        */
/*****************************************************************************/
init()
{
	failedNID = 0;

  /***************************************************************************/
	/* All imported functions must appear in this list.   This patches up the  */
  /* function calls to the correct addresses.                                */
  /***************************************************************************/
  resolveSyscall(&sceCtrlGetSamplingMode,0xda6b76a1L);
  resolveSyscall(&sceCtrlPeekBufferPositive,0x3a622550L);
  resolveSyscall(&sceCtrlReadBufferPositive,0x1f803938L);
  resolveSyscall(&sceCtrlSetSamplingMode,0x1f4011e6L);
  resolveSyscall(&sceDisplaySetFrameBuf,0x289d82feL);
  resolveSyscall(&sceDisplaySetMode,0x0e20f177L);
  resolveSyscall(&sceDisplayWaitVblankStart,0x984c27e7L);
  resolveSyscall(&sceIoClose,0x810c4bc3L);
  resolveSyscall(&sceIoDclose,0xeb092469L);
  resolveSyscall(&sceIoDopen,0xb29ddf9cL);
  resolveSyscall(&sceIoDread,0xe3eb004cL);
  resolveSyscall(&sceIoLseek,0x27eb27b8L);
  resolveSyscall(&sceIoMkdir,0x06a70004L);
  resolveSyscall(&sceIoOpen,0x109f50bcL);
  resolveSyscall(&sceIoRead,0x6a638d83L);
  resolveSyscall(&sceIoRemove,0xf27a9c51L);
  resolveSyscall(&sceIoRename,0x779103a0L);
  resolveSyscall(&sceIoRmdir,0x1117c65fL);
  resolveSyscall(&sceIoWrite,0x42ec03acL);
  resolveSyscall(&sceKernelCancelEventFlag,0xcd203292L);
  resolveSyscall(&sceKernelCancelSema,0x8ffdf9a2L);
  resolveSyscall(&sceKernelClearEventFlag,0x812346e4L);
  resolveSyscall(&sceKernelCreateCallback,0xe81caf8fL);
  resolveSyscall(&sceKernelCreateEventFlag,0x55c20a00L);
  resolveSyscall(&sceKernelCreateSema,0xd6da4ba1L);
  resolveSyscall(&sceKernelCreateThread,0x446d8de6L);
  resolveSyscall(&sceKernelDcacheWritebackAll,0x79d1c3faL);
  resolveSyscall(&sceKernelDcacheWritebackInvalidateRange,0x34b9fa9eL);
  resolveSyscall(&sceKernelDcacheWritebackRange,0x3ee30821L);
  resolveSyscall(&sceKernelDelayThread,0xceadeb47L);
  resolveSyscall(&sceKernelDeleteCallback,0xedba5844L);
  resolveSyscall(&sceKernelDeleteEventFlag,0xef9e4c70L);
  resolveSyscall(&sceKernelDeleteFpl,0xed1410e0L);
  resolveSyscall(&sceKernelDeleteSema,0x28b6489cL);
  resolveSyscall(&sceKernelDeleteThread,0x9fa03cd3L);
  resolveSyscall(&sceKernelDeleteVpl,0x89b3d48cL);
  resolveSyscall(&sceKernelExitDeleteThread,0x809ce29bL);
  resolveSyscall(&sceKernelExitThread,0xaa73c935L);
  resolveSyscall(&sceKernelFreeFpl,0xf6414a71L);
  resolveSyscall(&sceKernelFreeVpl,0xb736e9ffL);
  resolveSyscall(&sceKernelGetThreadId,0x293b45b8L);
  resolveSyscall(&sceKernelPollEventFlag,0x30fd48f0L);
  resolveSyscall(&sceKernelPollSema,0x58b1f937L);
  resolveSyscall(&sceKernelPowerTick,0x090ccb3fL);
  resolveSyscall(&sceKernelSetEventFlag,0x1fb15a32L);
  resolveSyscall(&sceKernelSignalSema,0x3f53e640L);
  resolveSyscall(&sceKernelStartThread,0xf475845dL);
  resolveSyscall(&sceKernelTerminateDeleteThread,0x383f7bccL);
  /*resolveSyscall(&sceKernelWaitEventFlag,0x402fcf22L);*/
  resolveSyscall(&sceKernelWaitSema,0x4e3a1105L);
  resolveSyscall(&sceKernelWaitThreadEnd,0x278c0df5L);
  resolveSyscall(&sceRtcGetCurrentTick,0x3f7ad767L);
  resolveSyscall(&vshCtrlReadBufferPositive,0xc6395c03L);
  resolveSyscall(&vshKernelExitVSHVSH,0x40716012L);
  resolveSyscall(&vshKernelLoadExecVSHMs1,0xf35bfb7dL);
  resolveSyscall(&sceUmdActivate,0xc6183d47L);
  resolveSyscall(&rand,0x60054052L);
  resolveSyscall(&sinf,0xe13b6daeL);
  resolveSyscall(&cosf,0xeb6d20a5L);
  resolveSyscall(&sceGuAlphaFunc,0xf99f7ffbL);
  resolveSyscall(&sceGuAmbient,0xe350525eL);
  resolveSyscall(&sceGuBlendFunc,0xc14ca370L);
  resolveSyscall(&sceGuCallList,0xa109d5dfL);
  resolveSyscall(&sceGuCallMode,0xba640d21L);
  resolveSyscall(&sceGuClear,0x388d8736L);
  resolveSyscall(&sceGuClearColor,0xd9f56690L);
  resolveSyscall(&sceGuClearDepth,0xa0547747L);
  resolveSyscall(&sceGuClearStencil,0x8f2316e1L);
  resolveSyscall(&sceGuClutLoad,0x29631777L);
  resolveSyscall(&sceGuClutMode,0x050a7eb0L);
  resolveSyscall(&sceGuColor,0x0f2615a4L);
  resolveSyscall(&sceGuCopyImage,0x05750a7cL);
  resolveSyscall(&sceGuInit,0x3624ae78L);
  resolveSyscall(&sceGuStart,0x5f01916fL);
  resolveSyscall(&sceGuDrawBuffer,0xcb7dc84fL);
  resolveSyscall(&sceGuDispBuffer,0xa79e81d7L);
  resolveSyscall(&sceGuDepthBuffer,0xe4a45c4aL);
  resolveSyscall(&sceGuEnable,0x7e0eaddeL);
  resolveSyscall(&sceGuFinish,0xba8cf6b9L);
  resolveSyscall(&sceGuOffset,0x178a6887L);
  resolveSyscall(&sceGuViewport,0xe4e2ad90L);
  resolveSyscall(&sceGuDepthRange,0xf0e6ee76L);
  resolveSyscall(&sceGuScissor,0xb8730cb7L);
  resolveSyscall(&sceGuDepthFunc,0xa24af273L);
  resolveSyscall(&sceGuFrontFace,0x68522d68L);
  resolveSyscall(&sceGuShadeModel,0x5c5f727cL);
  resolveSyscall(&sceGuLight,0x8d9b9a1aL);
  resolveSyscall(&sceGuLightColor,0x43e715b7L);
  resolveSyscall(&sceGuSync,0xe8d57dc6L);
  resolveSyscall(&sceGuDisplay,0x9b14983bL);
  resolveSyscall(&sceGumMatrixMode,0x9fd01858L);
  resolveSyscall(&sceGumLoadIdentity,0xeb46c9c0L);
  resolveSyscall(&sceGumPerspective,0x8f68eef4L);
  resolveSyscall(&sceGumTranslate,0x96610ae6L);
  resolveSyscall(&sceGumDrawArray,0xea4c24b3L);
  resolveSyscall(&sceGuSwapBuffers,0x1290e81aL);
  resolveSyscall(&sceGuTexMode,0x3a4d566bL);
  resolveSyscall(&sceGuTexFunc,0x16ab1034L);
  resolveSyscall(&sceGuTexFilter,0x3ba5c5ccL);
  resolveSyscall(&sceGuAmbient,0x16ab1034L);

  /***************************************************************************/
	/* Flush data cache.                                                       */
  /***************************************************************************/
	tiffsdkKernelDcacheWritebackAll();

  /***************************************************************************/
	/* We want to flush the ICache.  An easy way to do this is to flash the    */
  /* screen black and black.                                                 */
	/***************************************************************************/
	flashScreen(0, 0);
}

