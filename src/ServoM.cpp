#include "ServoM.h"

using namespace std;

ServoM::ServoM(int nb_pin, float min_duty, float max_duty){
    pin_ = nb_pin;
        
    min_ = min_duty;
    max_ = max_duty;
    
    // shit pin gestion: TODO
}

ServoM::~ServoM(){
    cout << "\tdest servo moteur" << endl;
    // liberation de la pin: TODO
}

float ServoM::angleToPWM(float angle) const{
    if (angle < 0){
        return min_;
    }
    if (angle > 180){
        return max_;
    }
    return (max_ - min_) / 180 + min_;
}

const float& ServoM::getAngle() const{
    return angle_;
}

void ServoM::rotate(float angle){
    angle_ = angleToPWM(angle);

    // shit motor gestion : TODO
}