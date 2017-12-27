#ifndef adc_h
#define adc_h
#include "MKL46Z4.h"

void initialize_adc(void);
void initialize_tpm(void);
void begin_conversion(void);
static uint32_t adc_r0_value = 0;
#endif
