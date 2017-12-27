/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    buttons.c
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "buttons.h"												//Declarations

/*----------------------------------------------------------------------------
  Function that initializes buttons
 *----------------------------------------------------------------------------*/
void buttonsInitialize(void){

	SIM->SCGC5 |=  SIM_SCGC5_PORTC_MASK; 					/* Enable clock for port C */
	PORTC->PCR[3] |= PORT_PCR_MUX(4);      	/* Pin PTC3 is GPIO */
	PTC->PDDR &= ~button_mask[button1];  /* enable PTC3 as Input */	
	PORTC->PCR[3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* TASK 2.9 - enable on PTC3 pull resistor, and choose correct pull option */
}
/*----------------------------------------------------------------------------
	Function reads button 1 state
*----------------------------------------------------------------------------*/
int32_t button1Read(){
	return FPTC->PDIR & button_mask[button1];						/* Get port data input register (PDIR) */
}
