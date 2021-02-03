#include <iostream>
#include <wiringPi.h>
#include "servo_motor.h"
#include <cmath>

int main(int argc, char const* argv[])
{
    wiringPiSetup(); // setup the library

    ServoMotor sm(22, 5, 21);

    std::cout << "Rotate to origin (0 rad)" << std::endl;
    sm.rotate(0);
    // sm.stop();

    std::cout << "Press Enter to continue...";
    std::cin.get();

    std::cout << "Rotate to pi rad" << std::endl;
    sm.rotate(M_PI/2);
    // sm.stop();
    std::cout << "Press Enter to continue...";
    std::cin.get();

    std::cout << "Rotate to pi rad" << std::endl;
    sm.rotate(-M_PI/2);
    sm.stop();
    std::cout << "Press Enter to continue...";
    std::cin.get();

    std::cout << "Rotate to origin (0 rad)" << std::endl;
    sm.rotate(0);
    // sm.stop();
    std::cout << "Press Enter to exit...";
    std::cin.get();
    sm.stop();
    return 0;
}
