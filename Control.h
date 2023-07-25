/*
 * Control.h
 *
 *  Created on: 2023年7月11日
 *      Author: luxy
 */

#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "main.h"
#include "tim.h"

void CarDataAnalysis(uint8_t Data[4]);
void Bluetooth_mode(uint8_t Data[4]);
void Track_mode(uint8_t Data[4]);
void Vision_mode(uint8_t Data[4]);

#endif /* INC_CONTROL_H_ */
