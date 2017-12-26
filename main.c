#include "MKL46Z4.h"
#include "slcd.h"
#include "transmitter.h"


int main(void) {	

	initialize_pin();
	filltab();
	for (;;) 
	{
		senddata();
	}	
}

