#include "ServoM.h"

ServoM::ServoM(int nb_pin, float min_duty=3, float max_duty=12){
    _pin = nb_pin;
    pinMode(_pin, OUTPUT);

    

}