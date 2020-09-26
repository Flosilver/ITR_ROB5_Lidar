#include "ServoM.hpp"

using namespace std;

ServoM::ServoM(int nb_pin, float min_duty=3, float max_duty=12){
    _pin = nb_pin;
        
    _min = min_duty;
    _max = max_duty;
    
    // shit pin gestion: TODO
}

ServoM::~ServoM(){
    cout << "\tdest servo moteur" << endl;
    // liberation de la pin: TODO
}

const float& ServoM::angleToPWM(float angle) const{
    if (angle < 0){
        return _min;
    }
    if (angle > 180){
        return _max;
    }
    return (_max - _min) / 180 + _min;
}

const float& ServoM::getAngle() const{
    return _angle;
}

void ServoM::rotate(float angle){
    _angle = angleToPWM(angle);

    // shit motor gestion : TODO
}