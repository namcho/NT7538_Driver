/*
 * LCDInterface.h
 *
 *  Created on: 10 Nis 2016
 *      Author: EN
 */

#ifndef LCDINTERFACE_H_
#define LCDINTERFACE_H_

#include "ihwlcd.h"

#define BUFFER_COMMAND_LEN		10
#define BUFFER_DATA_LEN			1024

void AssignPointerFunc(LCD_HW_Interface_t *lcdHwInterface);


#endif /* LCDINTERFACE_H_ */
