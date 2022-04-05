// include psp headers
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
 
 
#include <iostream>
#include <string>
#include <atomic>

// configure PSP stuff
#define VERS    1
#define REVS    0
 
PSP_MODULE_INFO("Hello World", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER|THREAD_ATTR_VFPU);

class test {
    public:
        test() {}
        ~test() {}
        void domsg(const std::string& msg);
    private:
        std::atomic<time_t> last_err_time_;
};

inline void test::domsg(const std::string& msg)
{
    auto now = time(nullptr);
    if (now - last_err_time_ < 60)
    {
        return;
    }
    last_err_time_ = now;

    std::cout << now << msg << std::endl;
}
 
int main()
{
    test t;
    t.domsg("awesome");
}