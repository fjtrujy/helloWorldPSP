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
	int __psp_heap_blockid;
	char totalFreeString[9] = {0};
	char maxFreeString[9] = {0};
	int block_id = 0;
	char blockString[2] = {0};

	void *heap_bottom = NULL;
	void *heap_top = NULL;
	
	itoa(totalFree, totalFreeString, 10);

	int fdout = sceIoOpen("ms0:/memory_info.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	if(fdout > 0)
	{
		sceIoWrite(fdout, "Summary\n", 8);
		
		sceIoWrite(fdout, "sceKernelTotalFreeMemSize: ", 27);
		sceIoWrite(fdout, totalFreeString, 9);
		sceIoWrite(fdout, "\n", 1);

		int maxFree = sceKernelMaxFreeMemSize();
		while (maxFree > 0) {
			itoa(block_id, blockString, 10);
			__psp_heap_blockid = sceKernelAllocPartitionMemory(PSP_MEMORY_PARTITION_USER, blockString, PSP_SMEM_High, maxFree, NULL);
			if (__psp_heap_blockid <= 0) {
				sceIoWrite(fdout, "ERROR HEAP!\n", 12);
				break;
			} else {
				itoa(maxFree, maxFreeString, 10);
				sceIoWrite(fdout, blockString, 2);
				sceIoWrite(fdout, " -> sceKernelMaxFreeMemSize: ", 29);
				sceIoWrite(fdout, maxFreeString, 9);

				heap_bottom = sceKernelGetBlockHeadAddr(__psp_heap_blockid);
				heap_top = (unsigned char *) heap_bottom + maxFree;
				sceIoWrite(fdout, " -> bottom: 0x", 14);
				itoa(heap_bottom, maxFreeString, 16);
				sceIoWrite(fdout, maxFreeString, 9);
				sceIoWrite(fdout, ", top: 0x", 9);
				itoa(heap_top, maxFreeString, 16);
				sceIoWrite(fdout, maxFreeString, 9);
				sceIoWrite(fdout, "\n", 1);

			maxFree = sceKernelMaxFreeMemSize();
			block_id++;
			}
		}
		sceIoClose(fdout);
	}

	sceKernelExitGame();
	return 0;
}

