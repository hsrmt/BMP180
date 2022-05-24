/*
 * BMP180.c
 *
 *  Created on: May 23, 2022
 *      Author: Hatice ŞEREMET
 */

#include "BMP180.h"

int16_t AC1;
int16_t AC2;
int16_t AC3;
int16_t AC4;
int16_t AC5;
int16_t AC6;
int16_t B1;
int16_t B2;
int16_t MB;
int16_t MC;
int16_t MD;

/*Temperature Values */
int16_t unCompTemperature;
float Temperature;
int16_t tX1,tX2,tB5;

/*Pressure Values */
int32_t unCompPresure;
float presure,presureATM;
int32_t pB6,pX1,pX2,pX3,pB3,pB4,pB7;


void BMP180_Init()
{
	if(HAL_I2C_IsDeviceReady(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 1, 100000)!=HAL_OK)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);

	BMP180_GetCalibration_Value();
}

void BMP180_GetCalibration_Value()
{
	int a=0;

	uint8_t calibBuff[BMP180_CALIBRATION_VALUE_LENGTH]={0};

	HAL_I2C_Mem_Read(&hi2c1,BMP180_DEVICE_READ_REGISTER_ADDRESS, BMP180_CALIBRATION_START_ADDRESS, 1, calibBuff, BMP180_CALIBRATION_VALUE_LENGTH, 100000);

	AC1 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	AC2 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	AC3 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	AC4 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	AC5 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	AC6 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	B1  = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	B2  = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	MB  = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	MC  = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
	MD  = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]);a+=2;
}

void BMP180_Get_Temperature(void)
{
	BMP180_Get_Uncompansated_Temperature_Value();
	tX1=(((int32_t)unCompTemperature - (int32_t)AC6)*(int32_t)AC5)/32768;
	tX2=((int32_t)MC*2048)/(tX1+(int32_t)MD);
	tB5= tX1+tX2;
	Temperature=(float)(tB5+8)/16*0.1;

}
void BMP180_Get_Uncompansated_Temperature_Value(void)
{
	uint8_t rData[2]={0};
	uint8_t wData[1];
	wData[0]=0x2E;
	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000);
	HAL_Delay(5);
	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0XF6, 1, rData, 2, 100000);
	unCompTemperature=(int16_t)((rData[0]<<8)|rData[1]);

}
void BMP180_Get_Presure(void)
{
	BMP180_Get_Uncompansated_Presure();
	pB6=tB5-4000;
	pX1=(B2*(pB6*pB6/4096))/2048;
	pX2=AC2*pB6/2048;
	pX3=pX1+pX2;
	pB3=(((AC1*4+pX3)<<(uint8_t)0x03)+2)/4;
	pX1=AC3*pB6/8192;
	pX2=(B1*(pB6*pB6/4096))/65536;
	pX3=((pX1+pX2)+2)/4;
	pB4=AC4*(signed long)(pX3+32768)/32768;
	pB7=((signed long)unCompPresure-pB3)*(50000>>(uint8_t)0x03);

	if(pB7<80000000)
		presure=(pB7*2)/pB4;
	else
		presure=(pB7/pB4)*2;

	pX1=(presure/256)*(presure/256);
	pX1=(pX1*3038)/65536;
	pX2=(-7357*presure)/65536;
	presure=-(presure+(pX1+pX2+3791)/16);
	presureATM=presure*0.00000986923;

}

void BMP180_Get_Uncompansated_Presure(void)
{
	uint8_t rData[3]={0};
	uint8_t wData[1];
	wData[0]=0x34 | (0x03 << 6);
	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000);
	HAL_Delay(26);
	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 3, 100000);
	unCompPresure=(rData[0]<<16	| rData[1] << 8 | rData[2])>>(8-(uint8_t)(0x03));

}
