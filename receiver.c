#include "slcd.h"
#include "leds.h"
#include <time.h>
#include <stdlib.h>

const static uint32_t receiver_mask = {1UL << 1};
const static uint32_t RedMask = {1UL << 29};  //Green led is Port D bit 5, Red led is Port E bit 29
	
void initialize_receiver(void) {
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK);														
	PORTC->PCR[1] = PORT_PCR_MUX(4);
	PORTC->PCR[1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* TASK 2.9 - enable on PTC3 pull resistor, and choose correct pull option */
	PTC->PDDR&=~receiver_mask;
	
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);	
	PORTE->PCR[29] = PORT_PCR_MUX(1UL);  /* Pin PTE29 is GPIO */
	PTE->PDDR |= RedMask;  /* enable PTE29 as Output */
	PTE->PSOR = RedMask;  /* switch Red LED off */
	
	
}


int32_t pinRead(){
	return FPTC->PDIR & receiver_mask;						/* Get port data input register (PDIR) */
}
void receive()
{
	if (pinRead()==0)
	{PTE->PCOR = RedMask;  /* switch Red LED off */
	}
	else
	{PTE->PSOR = RedMask;  /* switch Red LED on */
	}	
}