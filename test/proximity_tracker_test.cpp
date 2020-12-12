#include <iostream>
#include <memory>
#include "proximity_tracker.h"

int main(void)
{
    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->setOffset(encoder->measureIncrements());

    std::shared_ptr<MotorCtrl> motor(new MotorCtrl(0, 0x14, encoder));

    std::shared_ptr<IRSensor> sensor(new IRSensor(4, 1));

    std::shared_ptr<Lidar> lidar(new Lidar(motor, sensor));

    std::shared_ptr<ServoMotor> designator(new ServoMotor(17));

    ProximityTracker tracker(lidar, designator);

    std::cout << "Starting the tracker..." << std::endl;
    tracker.start();
    // Terminer le main
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
