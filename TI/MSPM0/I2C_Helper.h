/*
 * I2C_Helper.h
 *  MSPM0
 *  Created on: Feb 23, 2025
 *      Author: a0488796
 */

#ifndef I2C_HELPER_H_
#define I2C_HELPER_H_

#include "ti_msp_dl_config.h"

#define I2C_TX_MAX_PACKET_SIZE (512)

/* Maximum size of RX packet */
#define I2C_RX_MAX_PACKET_SIZE (4)



void I2C_setupHelper();
void I2C_WriteReg(I2C_Regs *i2cHandle, uint8_t target_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t count);
void I2C_ReadReg(I2C_Regs *i2cHandle, uint8_t target_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t count);

#endif /* I2C_HELPER_H_ */