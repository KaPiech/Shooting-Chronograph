#ifndef I2C_H
#define I2C_H

#include "frdm_bsp.h"

#define I2C_ERR_TIMEOUT		0x01 /* error = timeout */
#define I2C_ERR_NOACK			0x02 /* error = no ACK  */

void I2C_Init(void);
uint8_t I2C_Write(uint8_t address, uint8_t data);
	
#endif 
