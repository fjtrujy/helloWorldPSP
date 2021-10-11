#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
 

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>

// configure PSP stuff
#define VERS    1
#define REVS    0
 
PSP_MODULE_INFO("RetroArch", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER|THREAD_ATTR_VFPU);

char *itoa(int n, char *buf, int radix)
{
  char         *ret = buf;
  char         tmp[33];
  int          i = 0, j, r;

  /* validate the conversion number base. */
  if ((radix >= 2) && (radix <= 36)) {
    if ((radix == 10) && (n < 0)) {
      /* negative integer value. */
      *buf++ = '-';
      n = -n;
    }
    do {
      /* calculate the current digit. */
      r = (int)((unsigned int)n % radix);
      tmp[i++] = ((r < 10) ? (r + '0') : (r - 10 + 'a'));
    } while ((n /= radix) != 0);
    /* reverse the buffer string. */
    for (--i, j = 0; (i >= 0); --i, ++j) buf[j] = tmp[i];
    buf[j] = 0;
  }
  return (ret);
}
 
int _start(SceSize args, void *argp)
{
	int totalFree = sceKernelTotalFreeMemSize();
	int maxFree = sceKernelMaxFreeMemSize();
	char totalFreeString[9] = {0};
	char maxFreeString[9] = {0};
	itoa(totalFree, totalFreeString, 10);
	itoa(maxFree, maxFreeString, 10);

	int fdout = sceIoOpen("ms0:/memory_info.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	if(fdout > 0)
	{
		sceIoWrite(fdout, "Summary\n", 8);
		
		sceIoWrite(fdout, "sceKernelTotalFreeMemSize: ", 27);
		sceIoWrite(fdout, totalFreeString, 9);
		sceIoWrite(fdout, "\n", 1);

		sceIoWrite(fdout, "sceKernelMaxFreeMemSize: ", 25);
		sceIoWrite(fdout, maxFreeString, 9);
		sceIoWrite(fdout, "\n", 1);

		sceIoClose(fdout);
	}

	sceKernelExitGame();
	return 0;
}

