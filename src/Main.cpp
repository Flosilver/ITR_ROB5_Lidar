#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include <wiringPi.h>
#include <softPwm.h>

using namespace std;


int main(void){
    wiringPiSetup();     // setup the library
    cout << "wiring PI seted up\n";
    pinMode(22, OUTPUT);
    digitalWrite(22,LOW);
    cout << "pin mode seted up to OUTPUT with LOW value\n";

    if (softPwmCreate(22,0,100) != 0){
        cerr << "***ERROR: softPwmCreate/n";
    }
    softPwmWrite(22,0);
    while(1){
        softPwmWrite(22,4);
        delay(1000);

        softPwmWrite(22,22);
        delay(1000);
    }

    return 0 ;
}