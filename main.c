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
 
extern int _start(SceSize args, void *argp);
extern void *_end;

#define MB_SIZE 1024 * 1024
#define KB_SIZE 1024

#define SIZE_PATTERN 2

#if defined(SCREEN_DEBUG)
#define custom_printf(args...) \
    printf(args);              \
    pspDebugScreenPrintf(args);
#else
#define custom_printf(args...) printf(args);
#endif

static int exitRequest  = 0;

static int isRunning()
{
	return !exitRequest;
}

static int exitCallback(int arg1, int arg2, void *common) 
{ 
	exitRequest = 1; 
	return 0; 
} 

static int callbackThread(SceSize args, void *argp) 
{ 
	int callbackID; 

	callbackID = sceKernelCreateCallback("Exit Callback", exitCallback, NULL); 
	sceKernelRegisterExitCallback(callbackID); 

	sceKernelSleepThreadCB(); 

	return 0; 
} 

static int setupExitCallback() 
{ 
	int threadID = 0; 

	threadID = sceKernelCreateThread("Callback Update Thread", callbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0); 
	 
	if(threadID >= 0) 
	{ 
		sceKernelStartThread(threadID, 0, 0); 
	} 

	return threadID; 
}

static int max_malloc(size_t initial_value, int increment, const char *desc)
{
    char *p_block;
    size_t chunk = initial_value;

    custom_printf("Check maximum contigous block we can allocate (%s accurate)\n", desc);
    while ((p_block = malloc(++chunk * increment)) != NULL) {
        free(p_block);
    }
    chunk--;
#if defined(VERBOSE)
    custom_printf("Maximum possible %s we can allocate is %i\n", desc, chunk);
#endif

    return chunk;
}

static int mem_integrity(char *p_block, size_t initial_value, size_t end_value, int increment, const char *desc)
{
    unsigned char patterns[SIZE_PATTERN] = {0xA, 0x5};
    int i, j, failures;
    char *block_start, *block_tmp;
#if defined(VERBOSE)
    char *block_end;
#endif

    failures = 0;

    custom_printf("Checking %s chunks...\n", desc);
    for (i = initial_value; i < end_value; i++) {
        for (j = 0; j < SIZE_PATTERN; j++) {
            block_start = p_block + i * increment;
#if defined(VERBOSE)
            block_end = block_start + increment;
            custom_printf("Checking from %p to %p with pattern 0x%X\n", block_start, block_end, patterns[j]);
#endif
            memset(block_start, patterns[j], increment);
            for (int x = 0; x < increment; x++) {
                block_tmp = block_start + x;
                if (block_tmp[0] != patterns[j]) {
                    failures++;
                    custom_printf("Failure, mem pos: %p\n", block_tmp);
                    custom_printf("Expected value 0x%X, readed: 0x%X\n", patterns[j], block_tmp[0]);
                }
            }
        }
    }

    return failures;
}

int main(int argc, char** argv)
{
    int failures;
    size_t size_mb, size_kb, size_b;
    char *p_block;

    failures = 0;
    size_mb = 0;
    size_kb = 0;
    size_b = 0;

	// basic init
    setupExitCallback();
    
#if defined(SCREEN_DEBUG)
	pspDebugScreenInit();
	sceDisplayWaitVblankStart(); // wait for vblank
	pspDebugScreenClear(); // reset where we draw
    custom_printf("\n\nStarting MEM TESTS...\n");
#endif
	custom_printf("Program: [%p, %p], program size %i\n", &_start, &_end, (int)&_end - (int)&_start);
    custom_printf("Stack start at %p\n", &failures);

    size_mb = max_malloc(size_mb, MB_SIZE, "MiB");

    size_kb = size_mb * KB_SIZE;
    size_kb = max_malloc(size_kb, KB_SIZE, "KiB");

    size_b = size_kb * KB_SIZE;
    size_b = max_malloc(size_b, 1, "Bytes");

    custom_printf("Start memory integration\n");
    p_block = malloc(size_b);

    failures += mem_integrity(p_block, 0, size_mb, MB_SIZE, "MiB");
    failures += mem_integrity(p_block, size_mb * KB_SIZE, size_kb, KB_SIZE, "KiB");
    failures += mem_integrity(p_block, size_kb * KB_SIZE, size_b, 1, "Bytes");

    free(p_block);
    custom_printf("End memory integration\n");

    custom_printf("\n\n\n----------SUMMARY-----------\n");
    custom_printf("Maximum possible MB we can allocate is %i MiB\n", size_mb);
    custom_printf("Maximum possible KB we can allocate is %i KiB\n", size_kb);
    custom_printf("Maximum possible Bytes we can allocate is %i Bytes\n", size_b);
    if (failures) {
        custom_printf("Memory integrity: Opps :'( The memory failed checking bytes, %i times\n", failures);
    } else {
        custom_printf("Memory integrity: The memory is working as expected!\n");
    }
    custom_printf("----------------------------\n\n\n");

	int available;
	int block = 0;
	int total_extra = 0;
	char block_name[64];
	
	available = sceKernelMaxFreeMemSize();
	
	while(available > 0) {
		sprintf(block_name, "newlib_block_%i", block);
		if (sceKernelAllocPartitionMemory(PSP_MEMORY_PARTITION_USER, block_name, PSP_SMEM_Low, available, NULL) <= 0) {
			custom_printf("Error allocating!!\n")
		} else {
			total_extra += available;
		}

		available = sceKernelMaxFreeMemSize();
	}

	custom_printf("Extra available %i!!\n", total_extra);
	custom_printf("Total free %i!!\n", sceKernelTotalFreeMemSize());

	while(isRunning()) {}

   //  exit
    sceKernelExitGame();
    return 0;
}