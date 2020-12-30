#include <iostream>
#include <memory>
#include <wiringPi.h>
#include <atomic>
#include "proximity_tracker.h"

int main(void)
{
    wiringPiSetup(); // setup the library
    std::cout << "wiring PI seted up\n";

    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->setOffset(encoder->measureIncrements());

    std::shared_ptr<DCMotor> motor(new DCMotor(0, 0x14));

    std::shared_ptr<IRSensor> sensor(new IRSensor(4, 1));

    std::shared_ptr<Lidar> lidar(new Lidar(motor, encoder, sensor, -0.4, 0.4));

    std::shared_ptr<ServoMotor> designator(new ServoMotor(22, 5, 21));

    ProximityTracker tracker(lidar, designator);

    std::cout << "Starting the tracker..." << std::endl;
    tracker.start();

    // Terminer le main
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
