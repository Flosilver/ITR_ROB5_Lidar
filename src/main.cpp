#include <iostream>
#include <string>
#include <wiringPi.h>
#include "gmft_game.h"
#include "proximity_tracker.h"

void printHelp(std::ostream& out)
{
    out << "  --help\t Print this manual.\n";
    out << "  --proximity-tracker or -pt\n\tLaunch the proximity tracker program.\n";
    out << "  --grandmother-footstep or -gf\n\tLaunch the grandmother footstep grame.\n";
    out << "NOTE: --proximity-tracker and --grandmother-footstep are mutually exclusive.\n";
}

void parseArgs(int argc, const char* argv[], int& mode)
{
    mode = -1;
    // Arguments parsing
    int i(1);
    for (; i < argc; ++i)
    {
        std::string arg(argv[i]);
        if (arg == "--help")
        {
            std::cout << "Allowed options:\n";
            printHelp(std::cout);
            exit(0);
        }
        else if (arg == "--proximity-tracker" || arg == "-pt")
        {
            if (mode == -1) { mode = 0; }
            else
                break;
        }
        else if (arg == "--grandmother-footstep" || arg == "-gf")
        {
            if (mode == -1) { mode = 1; }
            else
                break;
        }
        else
            break;
    }
    if (argc == 1)
    {
        std::cout << "Allowed options:\n";
        printHelp(std::cout);
        exit(0);
    }
    if (i != argc)
    {
        std::cerr << "Invalid argument '" << argv[i] << "' given! Please read the manual.\n";
        printHelp(std::cerr);
        exit(1);
    }
}

int main(int argc, const char* argv[])
{
    int mode;
    parseArgs(argc, argv, mode);
    // Actual main program
    wiringPiSetup(); // setup the library
    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->setOffset(encoder->measureIncrements());
    std::shared_ptr<DCMotor> motor(new DCMotor(0, 0x14));
    std::shared_ptr<IRSensor> sensor(new IRSensor(4, 1));
    std::shared_ptr<Lidar> lidar(new Lidar(motor, encoder, sensor, -0.4, 0.4));
    if (mode == 0) // Proximity tracker
    {
        std::shared_ptr<CameraPanTilt> camera(new CameraPanTilt(0, 22, 0));
        ProximityTracker tracker(lidar, camera);
        tracker.start();
        std::cout << "Press <Enter> to terminate.\n";
        std::cin.get();
    }
    else if (mode == 1) // Grandmother footstep
    {
        std::shared_ptr<Referee> ref(new Referee(0, 22, 0, 0.5, 25., 3000));
        GMFTGame gmft(lidar, ref, 0.4, 0.1, 0.03);
        gmft.play();
        std::cout << "Press <Enter> to terminate.\n";
        std::cin.get();
    }
    else
    {
        std::cerr << "Invalid mode given. Please read the fabulous manual.\n";
        exit(2);
    }

    return 0;
}
