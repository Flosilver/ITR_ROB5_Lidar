#ifndef __GPIO_H__
#define __GPIO_H__

// Définition des macros pour l'utilisation directe des GPIO de la Raspberry Pi :
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define SET_GPIO_ALT(g, a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define GET_GPIO(g) (*(gpio+13)&(1<<g))

#endif // __GPIO_H__
