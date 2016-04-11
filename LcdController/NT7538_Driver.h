/*
 * NT7538_Driver.h
 *
 *  Created on: 8 Nis 2016
 *      Author: Nazým Yýldýz
 *      @brief: NT7538 has 132x65 = 8580bits ram area for drawing operations.
 */

#ifndef CONFIG_NT7538_DRIVER_H_
#define CONFIG_NT7538_DRIVER_H_

#include "stdint.h"
#include "ihwlcd.h"


/*
* @brief         : LCD icin ilk ayarlamalar yapiliyor.
* @param         lcdHardware: LCD ile islemcinin iletisim kurmasi icin kullanilan fonksiyonlar
* @return        : Yok
* @precondition  : Yok
* @postcondition : Yok
*/
void LCDInit(LCD_HW_Interface_t *lcdHardware);

/*
* @brief         : LCD yi acar
* @return        : Yok
* @precondition  : Yok
* @postcondition : Yok
*/
void LCDOn(LCD_HW_Interface_t *lcdHardware);

/*
* @brief         : LCD yi kapatir.
* @return        : Yok
* @precondition  : Yok
* @postcondition : Yok
*/
void LCDOff(LCD_HW_Interface_t *lcdHardware);
#endif /* CONFIG_NT7538_DRIVER_H_ */
