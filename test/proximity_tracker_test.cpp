#include <atomic>
#include <iostream>
#include <memory>
#include <wiringPi.h>
#include "proximity_tracker.h"

int main(void)
{
    wiringPiSetup(); // setup the library
    std::cout << "wiring PI seted up\n";

    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->offset(encoder->measureIncrements());

    std::shared_ptr<DCMotor> motor(new DCMotor(0, 0x14));

    std::shared_ptr<IRSensor> sensor(new IRSensor(4, 1));

    std::shared_ptr<Lidar> lidar(new Lidar(motor, encoder, sensor, -0.4, 0.4));

    std::shared_ptr<CameraPanTilt> camera(new CameraPanTilt(0, 22, 0));

    ProximityTracker tracker(lidar, camera);

    std::cout << "Starting the tracker..." << std::endl;
    tracker.start();

    // Terminer le main
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
