#include "transmitter.h"
#include "slcd.h"
#include "leds.h"

void initialize_pin(void) {
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK);														
	PORTD->PCR[3] = PORT_PCR_MUX(1UL);
	PORTD->PCR[5] = PORT_PCR_MUX(1UL);
	PTD->PDDR |= transmask2;	
	PTD->PSOR |= transmask2;
}