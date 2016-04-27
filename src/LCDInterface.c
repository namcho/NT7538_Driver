/*
 * LCDInterface.c
 *
 *  Created on: 10 Nis 2016
 *      Author: Nazým Yýldýz
 */

#include "LCDInterface.h"
#include "stm32f3xx_hal.h"
#include "math.h"

#define CPU_FREQ	72000000L
#define NANO_SEC_PER_TICK	13
#define INANO_SEC_PER_TICK	0.072046f		// 1/13.88 bolme yerine carpma yapmak icin

//PORT definations
#define CMD_CHIP_SELECT_PIN		GPIO_PIN_3
#define	CMD_RESET_PIN			GPIO_PIN_4
#define CMD_A0_PIN				GPIO_PIN_5
#define CMD_RW_PIN				GPIO_PIN_6
#define CMD_ENABLE_PIN			GPIO_PIN_7
#define COMMAND_PORT			GPIOB
#define DATA_PORT				GPIOE

//Buffers for operations
static int16_t commandBuffer[BUFFER_COMMAND_LEN];
static int16_t dataBuffer[BUFFER_DATA_LEN];

//Private helper functions
static void DataPortOut();
static void DataPortIn();
//Private interface functions.
static void WriteCommand(int16_t command);
static void WriteCommandM(int16_t *pCommand, uint16_t len);
static void WriteData(int16_t data);
static void WriteDataM(int16_t *pData, uint16_t len);
static int16_t ReadCommand();
static void ReadCommandM(int16_t *pCmdBuffer, uint16_t len);
static int16_t ReadData();
static void ReadDataM(int16_t *pDataBuffer, uint16_t len);

static void PinChipSelect(uint8_t HIGH_LOW);
static void PinEnable(uint8_t HIGH_LOW);
static void PinReset(uint8_t HIGH_LOW);
static void PinRW(uint8_t HIGH_LOW);
static void PinA0(uint8_t HIGH_LOW);		//For data or command selection. Indirect interface only.

static void PreInit();
static void PostInit();
static void Turnon();
static void Turnoff();
static void WaitingForOperation(uint32_t time_ns);

//
void AssignPointerFunc(LCD_HW_Interface_t *lcdHwInterface){
	lcdHwInterface->pfPinA0 		= PinA0;
	lcdHwInterface->pfPinChipSelect = PinChipSelect;
	lcdHwInterface->pfPinEnable		= PinEnable;
	lcdHwInterface->pfPinRW			= PinRW;
	lcdHwInterface->pfPinReset		= PinReset;

	lcdHwInterface->pfPostInit		= PostInit;
	lcdHwInterface->pfPreInit		= PreInit;
	lcdHwInterface->pfReadCommand	= ReadCommand;
	lcdHwInterface->pfReadCommandM	= ReadCommandM;
	lcdHwInterface->pfReadData		= ReadData;
	lcdHwInterface->pfReadDataM		= ReadDataM;
	lcdHwInterface->pfTurnoff		= Turnoff;
	lcdHwInterface->pfTurnon		= Turnon;
	lcdHwInterface->pfWriteCommand	= WriteCommand;
	lcdHwInterface->pfWriteCommandM	= WriteCommandM;
	lcdHwInterface->pfWriteData		= WriteData;
	lcdHwInterface->pfWriteDataM	= WriteDataM;

	lcdHwInterface->pfWaitingForOperation	= WaitingForOperation;
}
//-----------------
//Private functions
//------------------
void WriteCommand(int16_t command){
	//A0 = 0 : komut
	//RW = 0 : yazma
	PinA0(0);
	PinRW(0);
	WaitingForOperation(20);
	PinChipSelect(0);
	PinEnable(1);
	DATA_PORT->ODR &= 0xFF00;			//Portu temizle
	DATA_PORT->ODR |= (command & 0x00FF);
	WaitingForOperation(120);

	PinChipSelect(1);
	PinEnable(0);
	WaitingForOperation(120);
}

void WriteCommandM(int16_t *pCommand, uint16_t len){

}

void WriteData(int16_t data){

	PinA0(1);
	PinRW(0);
	WaitingForOperation(20);
	PinChipSelect(0);
	PinEnable(1);
	DATA_PORT->ODR &= 0xFF00;
	DATA_PORT->ODR |= (data & 0x00FF);
	WaitingForOperation(120);

	PinChipSelect(1);
	PinEnable(0);
	WaitingForOperation(120);
}

void WriteDataM(int16_t *pData, uint16_t len){

}

int16_t ReadCommand(){
	int16_t command;

	DataPortIn();
	PinA0(0);
	PinRW(1);
	PinEnable(1);
	WaitingForOperation(120);
	command = DATA_PORT->IDR & 0x00FF;
	PinEnable(0);
	WaitingForOperation(60);

	DataPortOut();
	return command;
}

void ReadCommandM(int16_t *pCmdBuffer, uint16_t len){

}

int16_t ReadData(){
	int16_t data;

	DataPortIn();
	PinA0(1);
	PinRW(1);
	WaitingForOperation(120);
	data = DATA_PORT->IDR & 0x00FF;
	PinEnable(0);
	WaitingForOperation(60);

	DataPortOut();
	return data;
}

void ReadDataM(int16_t *pDataBuffer, uint16_t len){

}

void PinChipSelect(uint8_t HIGH_LOW){
	if(HIGH_LOW == 1)
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_CHIP_SELECT_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_CHIP_SELECT_PIN, GPIO_PIN_RESET);
}

void PinEnable(uint8_t HIGH_LOW){
	if(HIGH_LOW == 1)
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_ENABLE_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_ENABLE_PIN, GPIO_PIN_RESET);
}

void PinReset(uint8_t HIGH_LOW){
	if(HIGH_LOW == 1)
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_RESET_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_RESET_PIN, GPIO_PIN_RESET);
}

void PinRW(uint8_t HIGH_LOW){
	if(HIGH_LOW == 1)
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_RW_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_RW_PIN, GPIO_PIN_RESET);
}
void PinA0(uint8_t HIGH_LOW){
	if(HIGH_LOW == 1)
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_A0_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(COMMAND_PORT, CMD_A0_PIN, GPIO_PIN_RESET);
}

void PreInit(){
	GPIO_InitTypeDef 	gpio;

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	//LCD Data pins
	gpio.Mode		= GPIO_MODE_OUTPUT_PP;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Pin		= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
						GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
						GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(DATA_PORT, &gpio);

	//Sirasiyla
	gpio.Pin		= CMD_A0_PIN | CMD_CHIP_SELECT_PIN | CMD_ENABLE_PIN | CMD_RESET_PIN |
						CMD_RW_PIN;
	HAL_GPIO_Init(COMMAND_PORT, &gpio);
}

void PostInit(){

}

void Turnon(){

}

void Turnoff(){

}
/*
* @brief         : LCD operation delay function. It should be implement RTOS_Delay for large systems.
* @param         time_ns: requested time delay in nano
* @return        : Yok
* @precondition  : Yok
* @postcondition : Yok
*/
void WaitingForOperation(uint32_t time_ns){
	uint32_t reqTick;
	float temp;

#ifndef INANO_SEC_PER_TICK
#error "INANO_SEC_PER_TICK must be defined!"
#endif

	temp = (float)time_ns * INANO_SEC_PER_TICK;
	reqTick = (uint32_t)ceilf(temp);	//Uste yuvarlama

	while(reqTick--);
}

//------------------------
//Private helper functions
//------------------------
void DataPortOut(){
	GPIO_InitTypeDef 	gpio;

	//LCD Data pins
	gpio.Mode		= GPIO_MODE_OUTPUT_PP;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Pin		= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
						GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
						GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(DATA_PORT, &gpio);
}

void DataPortIn(){
	GPIO_InitTypeDef 	gpio;

	//LCD Data pins
	gpio.Mode		= GPIO_MODE_INPUT;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Pin		= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
						GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
						GPIO_PIN_6 | GPIO_PIN_7;
	gpio.Pull		= GPIO_PULLDOWN;
	HAL_GPIO_Init(DATA_PORT, &gpio);
}
