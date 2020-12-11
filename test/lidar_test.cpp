#include "lidar.h"
#include <iostream>
#include <memory>
#include <vector>

int main(void)
{
    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->setOffset(encoder->measureIncrements());
    std::shared_ptr<MotorCtrl> motor(new MotorCtrl(0, 0x14, encoder));

    std::shared_ptr<IRSensor> sensor(new IRSensor(4,1));

    Lidar lidar(motor,sensor);

    std::vector<Lidar::Measure> mes;

    // Première update
    std::cout << "Press Enter to continue...";
    std::cin.get();

    mes = lidar.update();
    std::cout << "tableau des mesures:\n";
    for(Lidar::Measure& m : mes)
    {
        std::cout << "(" << m.pos << ";" << m.meas << "),";
    }
    std::cout << "\n";

    // Seconde update
    std::cout << "Press Enter to continue...";
    std::cin.get();

    mes = lidar.update(0.1,200);
    std::cout << "tableau des mesures:\n";
    for(Lidar::Measure& m : mes)
    {
        std::cout << "(" << m.pos << ";" << m.meas << "),";
    }
    std::cout << "\n";

    // Terminer le main
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;

}