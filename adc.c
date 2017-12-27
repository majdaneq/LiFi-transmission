#include "adc.h"
#include "slcd.h"
#include "leds.h"
uint32_t c = 0;
void ADC0_IRQHandler(void) {	
	adc_r0_value = (ADC0->R[0] * 0.7);			
	slcdDisplay(adc_r0_value,10);
	slcdSetDot(1);
	TPM0->SC |= TPM_SC_CMOD(0);
	TPM0->CONTROLS[2].CnV = (4095*adc_r0_value)/2800;
	TPM0->CONTROLS[5].CnV = (4095*adc_r0_value)/2800;	
	TPM0->SC |= TPM_SC_CMOD(1);
}
void begin_conversion(void) {
	c  = ADC0->SC1[0] & ~ADC_SC1_ADCH_MASK;
	c |= ADC_SC1_ADCH(3);
	ADC0->SC1[0] = c; 	
	delay_mc(100);
}
void initialize_adc(void) {
	uint32_t clpx_sum = 0;
	uint32_t clmx_sum = 0;
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;									//clock for ADC0
	ADC0->CFG1 |= ADC_CFG1_ADICLK(0);										//select clock for ADC0 -> Bus Clock = 24 MHz
	ADC0->CFG1 |= ADC_CFG1_ADIV(3);											//divide adc clock by 8 (then 3 MHz)
	ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK;									//ADC long sampling rate
	ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK;									//High Speed conversion sequence slected with 2 additional ADCK cycles to total convertson time
	ADC0->CFG2 |= ADC_CFG2_ADLSTS(0);										//20 extra ADCK cycles, 24 ADCK cycles total
	ADC0->SC2 |= ADC_SC2_REFSEL(0);											//external reference source in pin Vref, 3V Reference Voltage
	ADC0->SC3 |= ADC_SC3_AVGE_MASK;											//hardware average enabled
	ADC0->SC3 |= ADC_SC3_AVGS(3);												//hardware average for 32 samples	
	ADC0->SC3 |= ADC_SC3_CAL_MASK;											//enable calibration ADC	
	while ((ADC0->SC3 & ADC_SC3_CAL_MASK));							//wait for end of calibration	
	if ((ADC0->SC3 & ADC_SC3_CAL_MASK) == 0) {					//if end of calibration then			
		if ((ADC0->SC3 & ADC_SC3_CALF_MASK) == 0) {			
					clpx_sum = ADC0->CLP0 											//sum of ADC0->CLPx registers
										+ ADC0->CLP1 
										+ ADC0->CLP2 
										+ ADC0->CLP3 
										+ ADC0->CLP4 
										+ ADC0->CLPD 
										+ ADC0->CLPS;				
					clpx_sum /= 2;															//divide this sum by 2
					clpx_sum |= 0x8000;													//set MSB to 1
					ADC0->PG = ADC_PG_PG(clpx_sum);							//send sum clpx value to ADC0->PG
					clmx_sum = ADC0->CLM0												//sum of clmx registers
										+ ADC0->CLM1
										+ ADC0->CLM2
										+ ADC0->CLM3
										+ ADC0->CLM4
										+ ADC0->CLMD
										+ ADC0->CLMS;
					clmx_sum /= 2;															//divide this sum by 2						
					clmx_sum |= 0x8000;  												//set MSB to 1
					ADC0->MG = ADC_MG_MG(clmx_sum);												//send clmx sum value to ADC0->MG			
		}
	}		
	
	ADC0->CFG1 |= ADC_CFG1_ADICLK(1);										//select clock for ADC0 -> Bus Clock/2 = 12 MHz
	ADC0->CFG1 |= ADC_CFG1_ADIV(0);											//divide adc clock by 1 (then 12 MHz)
	ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK;									//ADC long sampling rate
	ADC0->CFG1 |= ADC_CFG1_MODE(1);											//12 bits conversion	
	ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;									//enable interrupt after conversion
	NVIC_ClearPendingIRQ(ADC0_IRQn);										//clear pending interrupt
	NVIC_EnableIRQ(ADC0_IRQn);													//enable nvic ADC0 interrupt
	
}


void initialize_tpm(void) {
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK 								//enable clock at port D and E
							| SIM_SCGC5_PORTE_MASK);
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;									//enable timer for TPM0;
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1)									//MCGFLLCLK clock or MCGPLLCLK/2					
							| SIM_SOPT2_PLLFLLSEL_MASK);						//MCGPLLCLK clock with fixed divide by two
	PORTE->PCR[29] = PORT_PCR_MUX(3);
	PORTD->PCR[5] = PORT_PCR_MUX(4);
	TPM0->SC |= TPM_SC_PS(7);												//prescaler for 128
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;
	TPM0->MOD = 4095;																		//12 bits
		
	TPM0->CONTROLS[2].CnSC &=  ~(TPM_CnSC_MSA_MASK 			//Edge Eligned PWM, High True Pulses
														| TPM_CnSC_ELSA_MASK);
	TPM0->CONTROLS[2].CnSC |=  (TPM_CnSC_ELSB_MASK 
														| TPM_CnSC_MSB_MASK);	
	TPM0->CONTROLS[5].CnSC &=  ~(TPM_CnSC_MSA_MASK 			//Edge Eligned PWM, Low True Pulses
														| TPM_CnSC_ELSB_MASK);
	TPM0->CONTROLS[5].CnSC |=  (TPM_CnSC_ELSA_MASK 
														| TPM_CnSC_MSB_MASK);
	TPM0->SC |= TPM_SC_CMOD(1);													//enable TPM0 timmer
	
}
