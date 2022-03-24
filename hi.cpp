#include <iostream>
#include <thread>
#include <chrono>

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>
 
PSP_MODULE_INFO("Hello", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER|THREAD_ATTR_VFPU);

int main()
{
    using namespace std::chrono_literals;
    std::cout << "Hello waiter\n" << std::flush;
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "Waited " << elapsed.count() << " ms\n";
}