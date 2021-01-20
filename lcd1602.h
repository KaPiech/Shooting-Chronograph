#ifndef LCD1602_H
#define LCD1602_H

#include "frdm_bsp.h"
#include "i2c.h"

void LCD1602_Init(void);
void LCD1602_Print(char *str);
void LCD1602_ClearAll(void);
void LCD1602_SetCursor(uint8_t col, uint8_t row);
void LCD1602_Backlight(uint8_t state);

#endif 
