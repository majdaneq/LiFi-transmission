#include "slcd.h"
#include "leds.h"
#include <time.h>
#include <stdlib.h>

const static uint32_t receiver_mask = {1UL << 1};
const static uint32_t RedMask = {1UL << 29};  //Green led is Port D bit 5, Red led is Port E bit 29
	
void initialize_receiver(void) {
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK);		
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);	
	
	PORTC->PCR[1] = PORT_PCR_MUX(4);
	PORTE->PCR[29] = PORT_PCR_MUX(1UL);  /* Pin PTE29 is GPIO */
	
	
		FPTE->PCOR = RedMask;  /* switch Red LED off */	
		FPTE->PDDR = RedMask;  /* enable PTE29 as Output */
	
	
	PORTC->PCR[1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* TASK 2.9 - enable on PTC3 pull resistor, and choose correct pull option */
	PTC->PDDR&=~receiver_mask;
	
}


int32_t pinRead(){
	return FPTC->PDIR & receiver_mask;						/* Get port data input register (PDIR) */
}
void receive()
{
	if (pinRead()==0)
	{PTE->PSOR = RedMask;  /* switch Red LED off */
	}
	else
	{PTE->PCOR = RedMask;  /* switch Red LED on */
	}	
}
uint32_t wynik_zmienna = 0, ilosc_probek = 1, dzielnik = 0, dzielnik2 = 0;
float wynik2 = 0;
uint32_t swiatlo = 0, temper = 0;
uint16_t wynik3 = 0;

void wynik_napiecie(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;	 //to zakomentowac
 // PORTD->PCR[6] = PORT_PCR_MUX(0);	//to zakomentowac
	
	//swiatlo = 0;
	//temper = 0;
	ADC0->SC1[0] = ADC_SC1_ADCH(7); // 29 nap ref+// 7 na pomiar napiecia
	wynik_zmienna+= ADC0->R[0] & ADC_R_D_MASK;
	
	dzielnik2++;
	if(dzielnik2 % 1 == 0)
	{
		wynik2 = ((wynik_zmienna / 1) / 16)* 3.3 / 4095 * 10;
		wynik3 = wynik2 * 4095 / 3.3 / 10;
		wynik_zmienna = 0;
		dzielnik2 = 0;
	}

	//TPM0->CONTROLS[2].CnV = wynik3;
	//TPM0->CONTROLS[5].CnV = wynik3;
}

void ADC_init(void)
{	
	uint16_t suma;
	
	SIM->SCGC6 = SIM_SCGC6_ADC0_MASK; //sygnal do modulu A/C	                                                                                                                          
	ADC0->CFG1 = ADC_CFG1_ADLSMP_MASK | ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(0) | ADC_CFG1_MODE(3) ; //dlugi czas probkowania, bus clock 24MHz / 2, dzielony na 1 = 12MHz, 16 bit

	ADC0->CFG2 = ADC_CFG2_ADHSC_MASK | ADC_CFG2_ADLSTS(1); //wspomaganie dla wysokich cz., 12 dodatkowych probek
	ADC0->SC2 = ADC_SC2_REFSEL(1); //referencja na zew. pinach Vref h i l
	ADC0->SC3 = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3); //wlacz usrednianie, usrednianie na 32 	
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK; //przerwania wlaczone na koniec konwersji	
	ADC0->SC3 = ADC_SC3_CAL_MASK; //wlacz kalibracje
	
	while((ADC0->SC3 & ADC_SC3_CAL_MASK) == 1){} //RedOn();			//Czekaj na koniec kalbracji
	
	//if((ADC0->SC3 & ADC_SC3_CALF_MASK) == 1) RedOn();				//Sprawdz, czy nie ma bledu
	//else
	//{
		//RedOff();
	//	GreenOn();
	//}

	suma = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS + ADC0->CLPD;
	suma = suma/2;
	suma = suma | 0x8000;
	//suma = suma | 0x2000;
	ADC0->PG = ADC_PG_PG(suma);
	
	suma = ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 + ADC0->CLM4 + ADC0->CLMS + ADC0->CLMD;
	suma = suma/2;
	suma = suma | 0x8000;
	//suma = suma | 0x2000;
	ADC0->MG = ADC_MG_MG(suma);	
	
	slcdDisplay(0, 10);
}

void wynik_na_LCD(void)
{
	dzielnik++;
	if(dzielnik % 100 == 0)
	{
		slcdDisplay(wynik2, 10);
			dzielnik = 0;
	
	
	}
}

