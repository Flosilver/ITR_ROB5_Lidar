#include <stdio.h>
#include <iostream.h>
#include <unistd.h>

#include <wiringPi.h>


int main(vois){
    wiringPiSetup();     // setup the library
    pinMode(1, PWM_OUTPUT);

    pwmSetClock(384); //clock at 50kHz (20us tick)
    pwmSetRange(1000); //range at 1000 ticks (20ms)
    pwmWrite(1, 75);  //theretically 50 (1ms) to 100 (2ms) on my servo 30-130 works ok
    return 0 ;
}