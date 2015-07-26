#include <pspkernel.h>
#include <pspsdk.h>
#include <pspctrl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


PspDebugRegBlock exception_regs;

extern SceModule module_info;
extern int _ftext;
void *_memset(void *buf, int ch, int n);

/*
static const char *codeTxt[32] =
{
    "Interrupt", "TLB modification", "TLB load/inst fetch", "TLB store",
    "Address load/inst fetch", "Address store", "Bus error (instr)",
    "Bus error (data)", "Syscall", "Breakpoint", "Reserved instruction",
    "Coprocessor unusable", "Arithmetic overflow", "Unknown 14",
    "Unknown 15", "Unknown 16", "Unknown 17", "Unknown 18", "Unknown 19",
    "Unknown 20", "Unknown 21", "Unknown 22", "Unknown 23", "Unknown 24",
    "Unknown 25", "Unknown 26", "Unknown 27", "Unknown 28", "Unknown 29",
    "Unknown 31"
};*/

static const unsigned char regName[32][5] =
{
    "zr", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"
};

extern void PSPEMU_Freeze(void);
extern int nesterj_menu(void);
extern u32 MessageBox(char *pszMsg, u32 color, u32 ulTypes);
#define RGB(r,g,b) ((((b>>3) & 0x1F)<<10)|(((g>>3) & 0x1F)<<5)|(((r>>3) & 0x1F)<<0)|0x8000)

void ExceptionHandler(PspDebugRegBlock * regs)
{
/*	if(setting.language == LANG_CHINESE)
	{
		if (MessageBoxGbk("模拟器严重出错，可能需要重启才能恢复，现在重启吗？", RGB(255, 0, 0), 2) == 1) 
		{
			sceKernelExitGame();
		}
	}else*/
	{
		if (MessageBox("Fatal error, the system need to restart, restart now?", RGB(255, 0, 0), 2) == 1) 
		{
			sceKernelExitGame();
		}
	
	}
}

void initExceptionHandler()
{
   SceKernelLMOption option;
   int args[2], fd, modid;

   _memset(&option, 0, sizeof(option));
   option.size = sizeof(option);
   option.mpidtext = PSP_MEMORY_PARTITION_KERNEL;
   option.mpiddata = PSP_MEMORY_PARTITION_KERNEL;
   option.position = 0;
   option.access = 1;

   if((modid = sceKernelLoadModule("exception.prx", 0, &option)) >= 0)
   {
      args[0] = (int)ExceptionHandler;
      args[1] = (int)&exception_regs;
      sceKernelStartModule(modid, 8, args, &fd, NULL);
   }
}
