/*
 * BMP180.h
 *
 *  Created on: May 23, 2022
 *      Author: Hatice ŞEREMET
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "stm32f4xx.h"

extern I2C_HandleTypeDef hi2c1;

/*BMP180 Device Addresses*/
#define BMP180_DEVICE_WRITE_REGISTER_ADDRESS 0xEE
#define BMP180_DEVICE_READ_REGISTER_ADDRESS 0xEF

/*BMP180 Calibration Address*/
#define BMP180_CALIBRATION_START_ADDRESS 0xAA

/*BMP180 Calibration Value Length*/
#define BMP180_CALIBRATION_VALUE_LENGTH 22



void BMP180_Init();
void BMP180_GetCalibration();
void BMP180_GetCalibration_Value();
void BMP180_Get_Uncompansated_Temperature_Value(void);
void BMP180_Get_Temperature(void);
void BMP180_Get_Uncompansated_Presure(void);
void BMP180_Get_Presure(void);

#endif /* INC_BMP180_H_ */
