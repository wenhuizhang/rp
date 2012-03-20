#include "coloshell.hpp"
#include "platform.h"
#include "program.hpp"
#include "beadisassembler.hpp"
#include "toolbox.hpp"
#include "XGetopt.hpp"

#include <iostream>
#include <exception>
#include <cstdlib>

#define NUM_V "0.1"

#ifdef ARCH_X64
#define VERSION_TMP NUM_V " x64 built the " __DATE__ " " __TIME__
#else
#define VERSION_TMP NUM_V " x86 built the " __DATE__ " " __TIME__
#endif

#ifdef WINDOWS
#define VERSION_TM VERSION_TMP " for Windows"
#else
#define VERSION_TM VERSION_TMP " for Unix"
#endif

#ifdef _DEBUG
#define VERSION VERSION_TM " (Debug)"
#else
#define VERSION VERSION_TM " (Release)"
#endif

void display_version()
{
    std::cout << "You are currently using the version " << VERSION << " of rp++." << std::endl;
}

void display_usage()
{
    std::cout << std::endl << "USAGE:" << std::endl << "./rp++ <option>\n" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t -f      : Give me the path of the binary" << std::endl;
    std::cout << "\t -d [0-2]: Display several information concerning the binary" << std::endl;
    std::cout << "\t           Specify the level of verbosity, 0 (default) to 2" << std::endl;
    std::cout << "\t -r <int>: Find a bunch of gadgets usable in your future exploits" << std::endl;
    std::cout << "\t           Specify the maximum number of instruction in your gadgets" << std::endl;
    std::cout << "\t -v      : Display the version of rp++ you are using" << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        display_usage();
        return -1;
    }

    int c;
    bool d_flag = false, r_flag = false, v_flag = false, f_flag = false;
    unsigned int display_value = 0, depth = 0;
    char* p_file = NULL;
    
    try
    {
        while ((c = getopt(argc, argv, "vr:d:f:")) != -1)
        {
            switch (c)
            {
                case 'v':
                    v_flag = true;
                    break;

                case 'r':
                    r_flag = true;
                    depth = atoi(optarg);
                    break;

                case 'd':
                    d_flag = true;
                    display_value = atoi(optarg);
                    break;

                case 'f':
                    f_flag = true;
                    p_file = optarg;
                    break;

                default:
                    continue;
            }
        }

        if(v_flag)
            display_version();
        
        if(f_flag)
        {
            std::string program_path(p_file);
            Program p(program_path);

            if(d_flag)
                p.display_information((display_value > 2)? VERBOSE_LEVEL_1 : (VerbosityLevel)display_value);

            if(r_flag)
                p.find_and_display_gadgets(depth);
        }
    }
    catch(const std::exception &e)
    {
        enable_color(COLO_RED);
        std::cout << e.what() << std::endl;
        disable_color();
    }

    return 0;
}