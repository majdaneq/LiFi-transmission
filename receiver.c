#include "slcd.h"
#include "leds.h"
#include <time.h>
#include <stdlib.h>

const static uint32_t receiver_mask = {1UL << 1};
const static uint32_t RedMask = {1UL << 29};  //Green led is Port D bit 5, Red led is Port E bit 29
	const static uint32_t GreenMask = {1UL << 29};
void initialize_receiver(void) {
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK);		
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);	
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	PORTD->PCR[5] = PORT_PCR_MUX(1UL); 
	PORTC->PCR[1] = PORT_PCR_MUX(4);
	PORTE->PCR[29] = PORT_PCR_MUX(1UL);  /* Pin PTE29 is GPIO */
	
			FPTD->PCOR = GreenMask;  /* switch Red LED off */	
		FPTD->PDDR = GreenMask;  /* enable PTE29 as Output */
		FPTE->PCOR = RedMask;  /* switch Red LED off */	
		FPTE->PDDR = RedMask;  /* enable PTE29 as Output */
	
	
	PORTC->PCR[1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* TASK 2.9 - enable on PTC3 pull resistor, and choose correct pull option */
	PTC->PDDR&=~receiver_mask;
	
}


int32_t pinRead(){
	return FPTC->PDIR & receiver_mask;						/* Get port data input register (PDIR) */
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
uint16_t vref= 0;
int check =0;
uint16_t vprev=0;
int tab[2000];
int j=0;
int k=0;
int max;
int min;
double clktime=0;
int clkset=0; 
void compare()
{
	wynik2*=10;
	//KOMPARACJA
	if (vref==0) //nieustalone
	{
		tab[j]=wynik2;
		j++;
			if (j ==1999)
			{	max =tab[0];
				min =tab[0];
			
				for(k=0;k<2000; k++)
				{
					if (tab[k]<min) min = tab[k];
					if (tab[k]>max) max =	tab[k];
				}				
				vref=(max+min)/2;
					ledsGreenOn();//PTD->PSOR = GreenMask;  /* switch Red LED on */
			//	ledgreenBlink();  /* switch Red LED off */
									
			}
			k=0;
	}


}

void clkrestore()
{
	if (clkset==0)
	{
	do 
	{
	clktime+=0.0208; //usek
	k++;
	}
	while (tab[k]>vref);
	clkset=1;	
	//clktime*=0.001;
	}	
	k=0;
	
}


int data_tabrec[2000];
int it =0;
int ramkaopenrec=0;
int jedn=0;
	int dzies=0;
	int setki=0;
	int tys=0;
void receive()
{	wynik_napiecie();

	compare();
	clkrestore();	
//	ledredBlink(1000,clktime*1000);
	//odbior na led
	if (wynik2>vref)
	{
		PTE->PSOR = RedMask;  /* switch Red LED off */	
		data_tabrec[it]=1;		
	}
	else 
	{
	PTE->PCOR = RedMask;  /* switch Red LED on */
	data_tabrec[it]=0;
	}
	delay_mc(clktime/2);
	
/*	if (it>3)
	{
		do
		{
			jedn = data_tabrec[it-3];
			dzies = data_tabrec[it-2] * 10;
			setki = data_tabrec[it-1]*100;
			tys=	data_tabrec[it]*1000;				
			ramkaopenrec =jedn+dzies+setki+tys;		
			
		}
		while (ramkaopenrec==1010 || it <2000);
	
		
		if (ramkaopenrec==1010 ||ramkaopenrec==0101)
	{ledsGreenOff();
	}
	}
*/	
	it++;
	if (it==1999)
		it =0;
//	if (pinRead()==0)
//	{PTE->PSOR = RedMask;  /* switch Red LED off */
//	}
//	else
//	{PTE->PCOR = RedMask;  /* switch Red LED on */
//	}	
}
