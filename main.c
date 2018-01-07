#include "MKL46Z4.h"
#include "adc.h"
#include "slcd.h"
#include "transmitter.h"
#include "receiver.h"
//For first set value of CLOCK_SETUP in system_MKL46Z4.c on 1
//MCG -> 8 MHz
//Core Clock -> 48 MHz
//Bus Clock ->24 MHz

void SysTick_Handler(void) { 							/* TBD_4.1:  Put the name of SysTick handler */	
	wynik_na_LCD();
}
int main(void) {	
//	uint32_t c =0;
	slcdInitialize();	
	ADC_init();
	
	initialize_receiver();
	initialize_pin();
	filltab();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 10000);

	
	for (;;) {		
//		begin_conversion();
	//	receive();
		senddata();
	//	wynik_napiecie();
	}	
}

