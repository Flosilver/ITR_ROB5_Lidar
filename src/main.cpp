#include <iostream>
#include <string>
#include <wiringPi.h>
#include "arguments.h"
#include "gmft_game.h"
#include "proximity_tracker.h"

int main(int argc, const char* argv[])
{
    // Parse the arguments.
    Mode mode;
    parseArgs(argc, argv, mode);
    wiringPiSetup(); // Setup the library.
    // Create common objects.
    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->offset(encoder->measureIncrements());
    std::shared_ptr<DCMotor> motor(new DCMotor(0, 0x14));
    std::shared_ptr<IRSensor> sensor(new IRSensor(4, 1));
    std::shared_ptr<Lidar> lidar(new Lidar(motor, encoder, sensor, -0.4, 0.4));
    // Launch the specified mode.
    if (mode == PROXIMITY_TRACKER)
    {
        std::shared_ptr<CameraPanTilt> camera(new CameraPanTilt(0, 22, 0));
        ProximityTracker tracker(lidar, camera);
        tracker.start();
        std::cout << "Press <Enter> to terminate.\n";
        std::cin.get();
    }
    else if (mode == GRANDMOTHER_FOOTSTEP)
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
        return 2;
    }
    return 0;
}
