#include "lidar.h"
#include <iostream>
#include <memory>
#include <list>

int main(void)
{
    std::shared_ptr<ShaftEncoder> encoder(new ShaftEncoder("/tmp/encoder", 420));
    encoder->offset(encoder->measureIncrements());
    std::shared_ptr<DCMotor> motor(new DCMotor(0, 0x14));
    std::shared_ptr<IRSensor> sensor(new IRSensor(4,1));
    Lidar lidar(motor,encoder,sensor,-0.5,0.5);

    std::list<Lidar::Measure> mes;

    lidar.start();

    // Première scan
    std::cout << "Press Enter to continue...";
    std::cin.get();

    mes = lidar.scan();
    std::cout << "Tableau des mesures: [";
    for(Lidar::Measure& m : mes)
    {
        std::cout << "(" << m.orientation << " ; " << m.distance << "), ";
    }
    std::cout << "]\n";

    // Seconde scan
    std::cout << "\nPress Enter to continue...";
    std::cin.get();

    mes = lidar.scan();
    std::cout << "Tableau des mesures: [";
    for(Lidar::Measure& m : mes)
    {
        std::cout << "(" << m.orientation << " ; " << m.distance << "), ";
    }
    std::cout << "]\n";

    // Terminer le main
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;

}
