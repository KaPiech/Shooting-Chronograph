#include "tpm.h"

static uint32_t counter_first_gate = 11;
static uint32_t counter_second_gate = 0;
static uint32_t counter_between_gates = 0;

void TPM1_Init_InputCapture(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;		//Enable TPM1 mask in SCGC6 register
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(0b01);	//Choose MCGFLLCLK clock source
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; 	//Connect port A to clock
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 	//Connect port B to clock
	
	PORTA->PCR[0] = PORT_PCR_MUX(2);  		//Set multiplekser to TPM1_CH0 for PTA0
	PORTB->PCR[5] = PORT_PCR_MUX(2);			//Set multiplekser to TPM1_CH1 for PTB13
	
	TPM1->SC |= TPM_SC_PS(0x6);  					//Set prescaler to 64
	TPM1->SC |= TPM_SC_CMOD(0b01);				//Select the internal input clock source
	TPM1->MOD = 0x0FA0;										//Set TPM1_MOD to 4000
	
	TPM1->SC &= ~TPM_SC_CPWMS_MASK; 			//up counting
	TPM1->CONTROLS[0].CnSC &= ~ (TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
	TPM1->CONTROLS[0].CnSC |= TPM_CnSC_ELSB_MASK; 	//capture on falling edge
	TPM1->CONTROLS[0].CnSC &= ~TPM_CnSC_ELSA_MASK;	//capture on falling edge
	
	TPM1->CONTROLS[1].CnSC &= ~ (TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_ELSB_MASK; 
	TPM1->CONTROLS[1].CnSC &= ~TPM_CnSC_ELSA_MASK;
  
	TPM1->CONTROLS[0].CnSC |= TPM_CnSC_CHIE_MASK; //Enable interrupt on channel 0
	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHIE_MASK; //Enable interrupt on channel 1
	
	NVIC_SetPriority(TPM1_IRQn, 1);  							//TPM1 interrupt priority level
	NVIC_ClearPendingIRQ(TPM1_IRQn); 
	NVIC_EnableIRQ(TPM1_IRQn);										//Enable Interrupts
}


uint32_t TPM1_GetVal(void) {
	return counter_between_gates;
}


void TPM1_IRQHandler(void) {
	
	if(counter_first_gate == 11)
		{ 
			TPM1->CNT = 0;																				//Set counter to 0 when first gate catch bullet
			counter_first_gate = TPM1->CONTROLS[1].CnV & 0xFFFF;  //Enable saving counter value
			TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK; 					//Clear channel flag
		}
		else
		{
			if(counter_second_gate == 0)
				{
					counter_second_gate = TPM1->CONTROLS[0].CnV & 0xFFFF;  							//Enable saving counter value
					counter_between_gates = counter_second_gate - counter_first_gate;		//Calculate counter between gates
					counter_first_gate = 11;																						//Reset gate counter
					counter_second_gate = 0;																						//Reset gate counter
					TPM1->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK; 												//Clear channel flag	
				}
		}
}

