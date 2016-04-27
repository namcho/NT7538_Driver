/*
 * NT7538_Driver.c
 *
 *  Created on: 8 Nis 2016
 *      Author: Nazým Yýldýz
 *		@note : 1) Parlakligi ayarlamak icin V0 voltaj regulator cikisi kontrol edilebilir.Bunun icin
 *				VEV voltaj referansi komut ile degistirilerek bu islem yapilir.
 *				2) Power Control registeri
 *				3) LCD Power Bias ayarlari
 *				4) V0 cikis geerilimi ayari
 *				5) V0 icin VEV gerilim referansi
 *				6)
 */
#include "NT7538_Driver.h"

//Veri transferlerinde kullanilacak olan tampon.
static uint8_t dataBuffer;
static void setDataBuffer(uint8_t val){	dataBuffer = val;	}
static uint8_t getDataBuffer(){	return dataBuffer;	}

//NT7538 icin bazi tanimlamalar
#define DISPLAY_OFF					(0)
#define DISPLAY_ON					(1)
#define COLUMN_ADR_HIGH				(1<<4)
#define COLUMN_ADR_LOW				(0)
#define ADCSELECT_HIGH				(1)
#define ADCSELECT_LOW				(0)
#define LCD_BIAS_HIGH				(1)
#define LCD_BIAS_LOW				(0)
#define COMMON_OUTPUT_MODE_HIGH		(1<<3)
#define COMMON_OUTPUT_MODE_LOW		(0)
#define OPERATION_STATUS_0			(0)
#define OPERATION_STATUS_1			(1)
#define OPERATION_STATUS_2			(2)
#define OPERATION_STATUS_3			(3)
#define OPERATION_STATUS_4			(4)
#define OPERATION_STATUS_5			(5)
#define OPERATION_STATUS_6			(6)
#define OPERATION_STATUS_7			(7)
#define STATIC_INDICATOR_ON			(1)
#define STATIC_INDICATOR_OFF		(0)
#define PART_ON						(1)
#define PART_SETTINGS				(2)
#define PART_OFF					(3)
#define OSCILLATION_FREQUENCY_26	(0)		//Kontrol
#define OSCILLATION_FREQUENCY_32	(1)		//Kontrol
#define PARTIAL_DISPLAY_MODE_ENTER		(0)	//Kontrol
#define PARTIAL_DISPLAY_MODE_RELEASE	(1) //Kontrol
#define POWER_CONTROL_MODE1				(0)	//External supply going to used.
#define POWER_CONTROL_MODE2				(1)	//Follower on
#define POWER_CONTROL_MODE3				(3)	//Regulator, Follower on
#define POWER_CONTROL_MODE4				(7)	//Booster, Follower, regulator on



//Private functions. These are commands taken from datasheet.
//Gonderilecek komutlar dataBuffer isimli degiskene yazilmaktadir.
static void Display(uint8_t DISPLAY_X);
static void SetPageAdr(uint8_t startAdr);
static void SetColumnAdr(uint8_t adr, uint8_t COLUMN_ADR_X);
static void ReadStatus();
static void ADCSelect(uint8_t ADCSELECT_X);
static void SetNormalDisplay();
static void SetReverseDisplay();
static void EntireDisplayOn();
static void EntireDisplayOff();
static void SetLcdBias(uint8_t LCD_BIAS_X);
static void ReadModifyWrite();
static void End();
static void Reset();
static void SetCommonOutputMode(uint8_t COMMON_OUTPUT_MODE_X);
static void SetPowerControl(uint8_t OPERATION_STATUS_X);
static void SetV0RegulatorResRatio(uint8_t resistor_ratio);
static void SetElectronicVolume(uint8_t alfa, uint8_t PART_X);
static void SetStaticIndicator(uint8_t STATIC_INDICATOR_X, uint8_t PART_X);
static void PowerSave();		//??
static void NOPLcd();
static void SetOscFreq(uint8_t OSCILLATION_FREQUENCY_X);	//Degerlere bakilcak...
static void SetPartialDisp(uint8_t PARTIAL_DISPLAY_MODE_X);
static void SetPartialDispDuty(uint8_t duty);
static void SetPartialDispBias(uint8_t bias);
static void SetPartialStartLine(uint8_t startLine);
static void SetNLine(uint8_t numofline);
static void SetNLineRelease();
static void SetDCDCClock(uint8_t division);
static void TestCommand();
static void TestModeReset();

//-------------------------------------------
//Public functions for GUI library
//-------------------------------------------
void LCDInit(LCD_HW_Interface_t *lcdHardware){
	//LCD'ye ilk acilista gecikme verme mikro port ayarlari vs...
	lcdHardware->pfPreInit();

	//Power stabilization watiting
	lcdHardware->pfPinReset(0);
	lcdHardware->pfWaitingForOperation(10000);	//tRW time

	lcdHardware->pfPinReset(1);
	lcdHardware->pfWaitingForOperation(1000);	//tR time

	//Chip select
	lcdHardware->pfPinChipSelect(1);
	lcdHardware->pfWaitingForOperation(20);

	//LCD Bias ayari YES icin 1/65 duty 1/9bias icin ayarlaniyor.
	//Bknz. NT7538 LCD BIAS SET komutu
	SetLcdBias(LCD_BIAS_LOW);
	lcdHardware->pfWriteCommand(getDataBuffer());

	//V0 gerilim bolucu ayari
	//Rb/Ra = 7.5 def degeri kullanilcak
	//VEV ayari
	//V0 = 8.5 volt icin a = 17 olarak hesaplandi.
	SetElectronicVolume(17, 0);
	lcdHardware->pfWriteCommand(getDataBuffer());
	SetElectronicVolume(17, 1);
	lcdHardware->pfWriteCommand(getDataBuffer());

	//PowerControl ayari
	//Voltage follower, voltage regulator, votlage booster are on.
	SetPowerControl(OPERATION_STATUS_7);
	lcdHardware->pfWriteCommand(getDataBuffer());

	Display(DISPLAY_ON);
	lcdHardware->pfWriteCommand(getDataBuffer());

	//Ekstra init icin yapilmasi istenenler
	lcdHardware->pfPostInit();
}

//Electrical LCDOn function
void LCDOn(LCD_HW_Interface_t *lcdHardware){

	lcdHardware->pfTurnon();
}

//Electrical LCDOff function
void LCDOff(LCD_HW_Interface_t *lcdHardware){

	lcdHardware->pfTurnoff();
}

//--------------------------------
//Private function implementations
//--------------------------------
void Display(uint8_t DISPLAY_X){
	setDataBuffer(0xAE | DISPLAY_X);
}


void SetPageAdr(uint8_t startAdr){
	setDataBuffer(0xB0 | startAdr);
}

void SetColumnAdr(uint8_t adr, uint8_t COLUMN_ADR_X){
	if(adr > 15)
		adr = 0;

	if(COLUMN_ADR_X == COLUMN_ADR_HIGH)
		setDataBuffer(0x10 | adr);
	else if(COLUMN_ADR_X == COLUMN_ADR_LOW)
		setDataBuffer(adr);
}

void ReadStatus(){

}

void ADCSelect(uint8_t ADCSELECT_X){
	setDataBuffer(0xA0 | ADCSELECT_X);
}

void SetNormalDisplay(){
	setDataBuffer(0xA6);
}

void SetReverseDisplay(){
	setDataBuffer(0xA7);
}

void EntireDisplayOn(){
	setDataBuffer(0xA5);
}

void EntireDisplayOff(){
	setDataBuffer(0xA4);
}

void SetLcdBias(uint8_t LCD_BIAS_X){
	setDataBuffer(0xA2 | LCD_BIAS_X);
}

void ReadModifyWrite(){
	setDataBuffer(0xE0);
}

void End(){
	setDataBuffer(0xEE);
}

void Reset(){
	setDataBuffer(0xE2);
}

void SetCommonOutputMode(uint8_t COMMON_OUTPUT_MODE_X){

}

void SetPowerControl(uint8_t OPERATION_STATUS_X){
	setDataBuffer(0x28 | OPERATION_STATUS_X);
}

void SetV0RegulatorResRatio(uint8_t resistor_ratio){
	if(resistor_ratio >  7)
		resistor_ratio = 4;		//Default value
	setDataBuffer(0x20 | resistor_ratio);
}

void SetElectronicVolume(uint8_t alfa, uint8_t PART_X){
	if(alfa > 63)
		alfa = 32;				//Default value

	if(PART_X == 0)
		setDataBuffer(0x81);
	else if(PART_X == 1)
		setDataBuffer(alfa);
}

void SetStaticIndicator(uint8_t STATIC_INDICATOR_X, uint8_t PART_X){
	if(PART_X == PART_ON)
		setDataBuffer(0xAD);
	else if(PART_X == PART_OFF)
		setDataBuffer(0xAC);
	else if(PART_X == PART_SETTINGS)
		setDataBuffer(STATIC_INDICATOR_X);

}

void PowerSave(){
	//Karmasik komut. Display ve EntireDisplay komutlariyla calisiyor..
}

void NOPLcd(){
	setDataBuffer(0xE3);
}

void SetOscFreq(uint8_t OSCILLATION_FREQUENCY_X){

}

void SetPartialDisp(uint8_t PARTIAL_DISPLAY_MODE_X){

}

void SetPartialDispDuty(uint8_t duty){

}

void SetPartialDispBias(uint8_t bias){

}

void SetPartialStartLine(uint8_t startLine){

}

void SetNLine(uint8_t numofline){

}

void SetNLineRelease(){

}

void SetDCDCClock(uint8_t division){

}

void TestCommand(){

}

void TestModeReset(){

}
