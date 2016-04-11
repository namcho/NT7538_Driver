/*
 * ihwlcd.h
 *
 *  Created on: 8 Nis 2016
 *      Author: EN
 */

#ifndef IHWLCD_H_
#define IHWLCD_H_
#include "stdint.h"

typedef struct _LCD_HARDWARE_t{
	void (*pfWriteCommand)(int16_t command);
	void (*pfWriteCommandM)(int16_t *pCommand, uint16_t len);
	void (*pfWriteData)(int16_t data);
	void (*pfWriteDataM)(int16_t *pData, uint16_t len);
	int16_t (*pfReadCommand)();
	void (*pfReadCommandM)(int16_t *pCmdBuffer, uint16_t len);
	int16_t (*pfReadData)();
	void (*pfReadDataM)(int16_t *pDataBuffer, uint16_t len);

	void (*pfPinChipSelect)(uint8_t HIGH_LOW);
	void (*pfPinEnable)(uint8_t HIGH_LOW);
	void (*pfPinReset)(uint8_t HIGH_LOW);
	void (*pfPinRW)(uint8_t HIGH_LOW);
	void (*pfPinA0)(uint8_t HIGH_LOW);		//For data or command selection. Indirect interface only.
	//Bunlar olmamali zayif baglasim icin....
	void (*pfPreInit)();
	void (*pfPostInit)();
	void (*pfTurnon)();
	void (*pfTurnoff)();

	void (*pfWaitingForOperation)(uint32_t time_ns);
}LCD_HW_Interface_t;

#endif /* IHWLCD_H_ */
