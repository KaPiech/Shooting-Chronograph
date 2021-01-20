#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "buttons.h"
#include "tpm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


float time;
float velocity_ms;
float velocity_fps;
float weight = 0.20;
float energy;
char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
int unit_on_lcd = 1;


int main (void)
{
	LCD1602_Init();													//LCD init
	LCD1602_Backlight(TRUE);
	LCD1602_Print("V");											//Control screen
	buttonsInitialize();										//Buttons init
	TPM1_Init_InputCapture ();							//TPM init
	
	while(1)
	{		
			time = ((TPM1_GetVal() * 0.00533)/4001);    //TPM_okres= 5.33[ms]  TPM_MOD= 4000
			velocity_ms = 0.072/time;										//Distance between gates = 72[mm]
			velocity_fps = velocity_ms * 3.28084;
			energy = ((velocity_ms * velocity_ms *(weight/1000))/2);
			
			LCD1602_ClearAll();
			LCD1602_SetCursor(0,0);
			if(unit_on_lcd == 1)
			{
				sprintf(display,"V[m/s]=%.1f",velocity_ms); 	
			}
			else
			{
				if(unit_on_lcd == -1)
					{
						sprintf(display,"V[fps]=%.1f",velocity_fps);
					}
			}
			LCD1602_Print(display);
			sprintf(display,"E=%.2fJ",energy);
			LCD1602_SetCursor(0,1);
			LCD1602_Print(display);
			sprintf(display,"m=%.2fg",weight);
			LCD1602_SetCursor(9,1);
			LCD1602_Print(display);
			
			__WFI();
	}
}

void unit_on_lcd_change()
{
	unit_on_lcd = -1 * unit_on_lcd;
}

void weight_update_up()
{	
	weight = weight + 0.01;
}

void weight_update_down()
{
	if(weight > 0.01)
	{
		weight = weight - 0.01;
	}
}





