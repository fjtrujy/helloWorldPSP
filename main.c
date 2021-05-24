#include <pspmoduleinfo.h>

PSP_MODULE_INFO("Hello World", 0, 1, 1);
// PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int printf(const char *format, ...);

int main()
{
   while (1)
   {
      printf("Hello, world!\n");
   }
   
   return 0;
}