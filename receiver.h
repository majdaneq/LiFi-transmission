#ifndef receiver_h
#define receiver_h
#include "MKL46Z4.h"


//const static uint32_t transmask2[] ={1UL << 5, 1UL << 3};
void initialize_receiver(void);
void receive(void);
void ADC_init(void);
void wynik_swiatlo(void);
void wynik_temperatura(void);
void wynik_na_LCD(void);
void wlacz_swiatlo(void);
void wlacz_temperature(void);
void odpal(void);
void wylacz(void);
void wynik_napiecie(void);

#endif
