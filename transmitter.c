#include "transmitter.h"
#include "slcd.h"
#include "leds.h"
#include <time.h>
#include <stdlib.h>


int tab[1000];
	int i=0;
void initialize_pin(void) {
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK);														
	PORTD->PCR[3] = PORT_PCR_MUX(1UL);
	PORTD->PCR[5] = PORT_PCR_MUX(1UL);
	PTD->PDDR |= transmask2;	
	PTD->PSOR |= transmask2;
}

void filltab()
{	for(i=0;i<1000;i++)
	{tab[i]=rand()%2;		
	}	
}

void togg(void)
{	delay_mc(100);
	PTD->PTOR |=transmask2;
}

void senddata()
{	for(i=0;i<1000;i++)
	{	if (tab[i]==0)
			PTD->PCOR |=transmask2;
		else 
			PTD->PSOR |=transmask2;

			delay_mc(50);
	}
}
