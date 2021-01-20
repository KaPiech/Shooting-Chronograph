#ifndef TPM_H
#define TPM_H

#include "frdm_bsp.h"

void TPM1_Init_InputCapture(void);
uint32_t TPM1_GetVal(void);
void TPM1_IRQHandler(void);

#endif 
