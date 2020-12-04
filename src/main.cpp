#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "itr.h"

using namespace std;

int main()
{
    wiringPiSetup(); // setup the library
    cout << "wiring PI seted up\n";

    DCMotor dcm(0,0x14);  // moteur DC sur la prise A = 1 et d'adresse 0x14
    int speed = 50;
    for(int i=0 ; i<10 ; i++){
        dcm.run_cw(speed);
        delay(500);
        dcm.motor_stop();
        dcm.run_ccw(speed);
        delay(500);
        dcm.motor_stop();
    }

    return 0;
}
