// include psp headers
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>
 
 
// configure PSP stuff
#define VERS    1
#define REVS    0
 
PSP_MODULE_INFO("HelloWorld", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
 
int main(int argc, char** argv)
{
    while(1) {
        printf("Hello World!\n"); // print some text
    }
    
    return 0;
}