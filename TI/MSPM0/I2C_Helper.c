/*
 * I2C_Helper.c
 *  MSPM0
 *  Created on: Feb 12, 2025
 *      Author: a0488796
 */


#include "I2C_Helper.h"
#include "ti_msp_dl_config.h"

/* Data sent to the Target */
uint8_t gTxPacket[I2C_TX_MAX_PACKET_SIZE];
/* Counters for TX length and bytes sent */
uint32_t gTxLen, gTxCount;

/* Data received from Target */
uint8_t gRxPacket[I2C_RX_MAX_PACKET_SIZE];
/* Counters for TX length and bytes sent */
uint32_t gRxLen, gRxCount;
/* Indicates status of I2C */
enum I2cControllerStatus {
    I2C_STATUS_IDLE = 0,
    I2C_STATUS_TX_STARTED,
    I2C_STATUS_TX_INPROGRESS,
    I2C_STATUS_TX_COMPLETE,
    I2C_STATUS_RX_STARTED,
    I2C_STATUS_RX_INPROGRESS,
    I2C_STATUS_RX_COMPLETE,
    I2C_STATUS_ERROR,
} gI2cControllerStatus;

// Sets up the default transaction
void I2C_setupHelper() {
    for (uint16_t i = 0; i < I2C_TX_MAX_PACKET_SIZE; i++) {
        gTxPacket[i] = 0;
    }
}

void I2C_WriteReg(I2C_Regs *i2cHandle, uint8_t target_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t count) {
    gTxPacket[0] = reg_addr;
    for (uint16_t i = 0; i < count; i++) {
        gTxPacket[i+1] = reg_data[i];
    }
    DL_I2C_fillControllerTXFIFO(i2cHandle, &gTxPacket[0], count);
    
    while (!(
        DL_I2C_getControllerStatus(i2cHandle) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
    
    DL_I2C_startControllerTransfer(i2cHandle, target_addr,
        DL_I2C_CONTROLLER_DIRECTION_TX, count);

    while (DL_I2C_getControllerStatus(i2cHandle) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    if (DL_I2C_getControllerStatus(i2cHandle) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        __BKPT(0);
    }

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(i2cHandle) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
}

void I2C_ReadReg(I2C_Regs *i2cHandle, uint8_t target_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t count) {
    uint8_t empty[1] = {0};
    I2C_WriteReg(i2cHandle, target_addr, reg_addr, empty, 0);

    DL_I2C_startControllerTransfer(i2cHandle, target_addr,
        DL_I2C_CONTROLLER_DIRECTION_RX, count);

    for (uint16_t i = 0; i < count; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(i2cHandle))
            ;
        reg_data[i] = DL_I2C_receiveControllerData(i2cHandle);
    }
    // if(I2C_transfer(i2cHandle, &g_i2cTransaction)) {
    //     for (uint8_t i = 0; i < count; i++) {
    //         reg_data[i] = rxBuffer[i];
    //     }
    // } else {
    //     for (uint8_t i = 0; i < count; i++) {
    //         reg_data[i] = 0xFF;
    //     }
    // }
}