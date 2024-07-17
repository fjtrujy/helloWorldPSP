#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <sys/types.h>

// include psp headers
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>
 
// configure PSP stuff
#define VERS    1
#define REVS    0
 
PSP_MODULE_INFO("GProfExample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER|THREAD_ATTR_VFPU);
 
 
// make printing easier on us
// #define printf pspDebugScreenPrintf

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

// Function to check if a number is prime
bool is_prime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

// Function to compute the sum of square roots of the first N prime numbers
double sum_of_square_roots(int N) {
    int count = 0;
    int num = 2;
    double sum = 0.0;

    while (count < N) {
        if (is_prime(num)) {
            sum += sqrt(num);
            count++;
        }
        num++;
    }
    return sum;
}

int dummy_function()
{
   int i;
   for (i = 0; i < 10000; i++)
   {
      printf(".");
   }
   printf("\n");
   return 0;
}

int main()
{
	// basic init
	setupExitCallback();

   printf("Hello, world!\n");
   dummy_function();  
   int N = 10000; // Large number of primes to compute
    printf("Sum of square roots of the first %d prime numbers is %lf\n", N, sum_of_square_roots(N));
   printf("Goodbye, world!\n");

   return 0;
}