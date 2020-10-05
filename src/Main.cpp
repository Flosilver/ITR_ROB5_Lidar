#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>

using namespace std;



int main(void){
    wiringPiSetup();     // setup the library
    cout << "wiring PI seted up\n";

    return 0 ;
}