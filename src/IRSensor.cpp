#include "IRSersor.h"

using namespace std;

IRSensor::IRSensor(int nb_pin){
    pin_ = nb_pin;

    // shit pin gestion: TODO
}

IRSensor::~IRSensor(){
    cout << "\tdest IR sensor" << endl;
    // liberation de la pin: TODO
}

const float& IRSensor::measure() const{
    // shit analog to digital convertor shit: TODO
}