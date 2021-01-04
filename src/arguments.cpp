#include "arguments.h"

void printHelp(std::ostream& out)
{
    out << "  --help\t Print this manual.\n";
    out << "  --proximity-tracker or -pt\n\tLaunch the proximity tracker program.\n";
    out << "  --grandmother-footstep or -gf\n\tLaunch the grandmother footstep grame.\n";
    out << "NOTE: --proximity-tracker and --grandmother-footstep are mutually exclusive.\n";
}

void parseArgs(int argc, const char* argv[], Mode& mode)
{
    mode = INVALID;
    // Arguments parsing
    int i(1);
    for (; i < argc; ++i)
    {
        std::string arg(argv[i]);
        if (arg == "--help")
        {
            std::cout << "Allowed options:\n";
            printHelp(std::cout);
            exit(0); // Job done, exit the program.
        }
        else if (arg == "--proximity-tracker" || arg == "-pt")
        {
            if (mode == INVALID) { mode = PROXIMITY_TRACKER; }
            else
                break; // The mode is already set.
        }
        else if (arg == "--grandmother-footstep" || arg == "-gf")
        {
            if (mode == INVALID) { mode = GRANDMOTHER_FOOTSTEP; }
            else
                break; // The mode is already set.
        }
        else
            break; // This argument is not valid.
    }
    if (argc == 1) // No argument given, print some help and exit.
    {
        std::cout << "Allowed options:\n";
        printHelp(std::cout);
        exit(0);
    }
    if (i != argc) // The parsing has terminated too early, there is an invalid argument.
    {
        std::cerr << "Invalid argument '" << argv[i] << "' given! Please read the manual.\n";
        printHelp(std::cerr);
        exit(1);
    }
}
