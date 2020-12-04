#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "itr.h"

using namespace std;

float moyenne(float* tab, int len) {
    float s = 0;
    for(int i=0 ; i<len ; i++){
        s += tab[i];
    }
    return s / len;
}

int main()
{
    wiringPiSetup(); // setup the library
    cout << "wiring PI seted up\n";

    /*DCMotor dcm(0,0x14);  // moteur DC sur la prise A = 1 et d'adresse 0x14
    int speed = 100;
    for(int i=0 ; i<4 ; i++){
        dcm.run_cw(speed);
        delay(500);
        dcm.motor_stop();
        //dcm.run_ccw(speed);
        //delay(500);
        //dcm.motor_stop();
    }*/

    IRSensor sensor(4, 1);
    int len = 200;
    float mes[len];
    for (int i=0 ; i<len ; i++){
        mes[i] = sensor.measure();
        cout << "mes: " << mes[i] << "\n";
        delay(100);
    }
    cout << "moyenne: " << moyenne(mes,len) << "\n";

    return 0;
}
