#include "MKL46Z4.h"
#include "adc.h"
#include "slcd.h"
#include "transmitter.h"
//For first set value of CLOCK_SETUP in system_MKL46Z4.c on 1
//MCG -> 8 MHz
//Core Clock -> 48 MHz
//Bus Clock ->24 MHz


int main(void) {	
//	uint32_t c =0;
//	slcdInitialize();	
//	initialize_adc();
//	initialize_tpm();
	initialize_pin();
	filltab();
	for (;;) {		
//		begin_conversion();
		senddata();
	}	
}

