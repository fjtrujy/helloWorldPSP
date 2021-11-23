#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>
 
PSP_MODULE_INFO("Hello", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER|THREAD_ATTR_VFPU);

std::mutex mprinter;
void printer(const char *msg)
{
	mprinter.lock();
	std::cout<<msg<<std::endl;
	mprinter.unlock();
}

void test3()
{
	std::thread t1(printer, "Hello 1");
	std::thread t2(printer, "Hello 2");
	std::thread t3(printer, "Goodbye!");

	t1.join();
	t2.join();
	t3.join();
}

int main()
{
    test3();
}
