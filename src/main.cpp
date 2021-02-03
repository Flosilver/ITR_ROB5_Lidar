#include <iostream>
#include <string>
#include <wiringPi.h>
#include "arguments.h"
#include "gmft_game.h"
#include "lidar_continuous.h"
#include "lidar_step.h"
#include "proximity_tracker.h"
#include "stoped_lidar.h"

int main(int argc, const char* argv[])
{
    // Parse the arguments.
    Arguments args(argc, argv);
    wiringPiSetup(); // Setup the library.
    // Create common objects.
    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 267));
    encoder->offset(encoder->measureIncrements());
    std::shared_ptr<IRSensor> sensor(new IRSensor(4, 1));

    // Build the specified lidar.
    int scan_cooldown(1000);
    std ::shared_ptr<Lidar> lidar;
    if (args.lidar() == Arguments::Lidar::STEP)
    {
        std::shared_ptr<MotorCtrl> motor(new MotorCtrl(0, 0x14, encoder));
        lidar = std::shared_ptr<Lidar>(new LidarStep(motor, sensor, -M_PI / 4, M_PI / 4));
        scan_cooldown = 1;
    }
    else if (args.lidar() == Arguments::Lidar::CONTINUOUS) // By default, use a continuous motor
    {
        std::shared_ptr<DCMotor> motor(new DCMotor(0, 0x14));
        lidar = std::shared_ptr<Lidar>(new LidarContinuous(motor, encoder, sensor, -M_PI / 4, M_PI / 4, 100));
    }
    else
    {
        std::shared_ptr<DCMotor> motor(new DCMotor(0, 0x14));
        lidar = std::shared_ptr<Lidar>(new StopedLidar(motor, encoder, sensor, 75));
    }

    // Launch the specified mode.
    if (args.mode() == Arguments::Mode::PROXIMITY_TRACKER)
    {
        std::shared_ptr<CameraPanTilt> camera(new CameraPanTilt(0, 22, 0));
        ProximityTracker tracker(lidar, camera, scan_cooldown);
        tracker.start();
        std::cout << "Press <Enter> to terminate.\n";
        std::cin.get();
    }
    else if (args.mode() == Arguments::Mode::GRANDMOTHER_FOOTSTEP)
    {
        std::shared_ptr<Referee> ref(new Referee(0, 22, 0, 0.5, 15., 500));
        GMFTGame gmft(lidar, ref, 0.4, 0.1, 0.05, 0.3);
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
