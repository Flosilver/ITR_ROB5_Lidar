#include <iostream>
#include <wiringPi.h>
#include "referee.h"

int main()
{
    wiringPiSetup(); // setup the library
    std::cout << "wiring PI seted up\n";

    Referee ref(0, 22, 0, 0.5, 25.);

    std::cout << "Starting the referee..." << std::endl;
    ref.display();

    for (int i = 0; i < 3; i++)
    {
        std::cout << "---------\nPress Enter to go to right target:\n";
        std::cin.get();
        ref.goToTarget(0);

        std::cout << "Press Enter to go to left target:\n";
        std::cin.get();
        ref.goToTarget(1);
    }

    std::cout << "Press Enter to exit...\n";
    std::cin.get();
    std::cout << ref.has_lost() << std::endl;
    return 0;
}
