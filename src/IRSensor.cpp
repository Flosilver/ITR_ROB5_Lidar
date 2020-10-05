#include "IRSersor.h"

using namespace std;

IRSensor::IRSensor(int addr, int nb_pin){
    address_ = addr;
    pin_ = nb_pin;

    // shit pin gestion: TODO
    fd_ = wiringPiI2CSetup(address_);
    if (fd == -1){
        cerr << "***ERROR: wiringPiI2CSetup(1) failed at" << address_ << "\n";
        // possible gestion de l'erreur
    }
}

IRSensor::~IRSensor(){
    cout << "\tdest IR sensor" << endl;
    // liberation de la pin: TODO
}

double IRSensor::measure() const{
    // shit analog to digital convertor shit: TODO
    int reg = address_ + 30;
    int res = 0;

    wiringPiI2CWrite(fd_, address_);
    res = wiringPiI2CReadReg16(fd_, address_);
    return (double) res / 10.;
}