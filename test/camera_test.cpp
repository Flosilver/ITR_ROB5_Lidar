#include <iostream>
#include <wiringPi.h>
#include "camera_pan_tilt.h"

int main(void)
{
    wiringPiSetup(); // setup the library
    CameraPanTilt cam(0, 22, 0);
    cam.rotatePan(0);
    cam.rotateTilt(0);
    cam.display();
    // Terminer le main
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}
