#include "transmitter.h"
#include "slcd.h"
#include "leds.h"
#include <time.h>
#include <stdlib.h>

const uint32_t MaskLED = {1UL << 5};
const static uint32_t transmask2 = {1UL << 3};
int tab[1000];
int ramka_tab[20];
int i=0;
	
	
void initialize_pin(void) {
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK);														
	PORTD->PCR[3] = PORT_PCR_MUX(1UL);
	PORTD->PCR[5] = PORT_PCR_MUX(1UL);	
	PTD->PDDR |= transmask2;	
	PTD->PSOR |= transmask2;	
	PTD->PDDR |= MaskLED;
	PTD->PSOR |=MaskLED;
}

void filltab()									//wypelnienie tablicy danych
{	
	for(i=0;i<1000;i++)
	{tab[i]=rand()%2;		
	}	
}

void ramkaopen()
{
	for(i=0;i<5;i++)
	ramka_tab[i]=1;	
	for(i=5;i<10;i++)
	ramka_tab[i]=0;
	for(i=10;i<15;i++)
	ramka_tab[i]=1;	
	for(i=15;i<20;i++)
	ramka_tab[i]=0;
}

void ramkaclose()
{
	for(i=0;i<5;i++)
	ramka_tab[i]=0;	
	for(i=5;i<10;i++)
	ramka_tab[i]=1;	
	for(i=10;i<15;i++)
	ramka_tab[i]=0;	
	for(i=15;i<20;i++)
	ramka_tab[i]=1;	
}

void togg(void)
{	
	delay_mc(100);
	PTD->PTOR |=transmask2;
}

void senddata()
{	
	ramkaopen();
	for (i=0;i<20;i++)											//start
		{	
			if (ramka_tab[i]==0)
					PTD->PCOR |=transmask2;
			else 
					PTD->PSOR |=transmask2;
			delay_mc(100);
		}	
		PTD->PCOR|=MaskLED;
	for(i=0;i<1000;i++)											//dane
		{	
			if (tab[i]==0)
				PTD->PCOR |=transmask2;
			else 
				PTD->PSOR |=transmask2;
			
			delay_mc(100);
		}
		ramkaclose();
		PTD->PSOR|=MaskLED;
		for (i=0;i<20;i++)										//stop
		{	
			if (ramka_tab[i]==0)
					PTD->PCOR |=transmask2;
			else 
					PTD->PSOR |=transmask2;
			delay_mc(100);
		}
	
}

 
void delay_mc(uint32_t value){
	uint32_t delay, x;	
	for(x=0; x < value; x++){
		for(delay=0; delay < 10000; delay++){};
	}
}
