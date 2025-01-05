/********************************************************************************************
*                                                                                           *
*                                          bsp.hpp                                          *
*                                                                                           *
*                             Copyright 2024 Jenish Rudani                          *
*                                                                                           *
* ----------------------------------------------------------------------------------------- *
*                                                                                           *
* This software is free software; you can redistribute it and/or modify it under the terms  *
* of the GNU General Public License as published by the Free Software Foundation; either    *
* version 3 of the License, or (at your option) any later version.                          *
*                                                                                           *
* This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;*
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. *
* See the GNU General Public License for more details.                                      *
*                                                                                           *
* You should have received a copy of the GNU General Public License along with this         *
* software. If not, see <https://www.gnu.org/licenses/>.                                    *
*                                                                                           *
* ----------------------------------------------------------------------------------------- *
*                                                                                           *
* ----------------------------------------------------------------------------------------- *
* Revision history:                                                                         *
* Created: 01.01.2025 20:32:47                                                              *
********************************************************************************************/
#ifndef BSP_H
#define BSP_H

#include "pico/stdlib.h"

#define LED_PIN         26

void BSP_init();
void BSP_start();

// Helper functions
void BSP_ledOn();
void BSP_ledOff();
void BSP_terminate(int16_t result);

#endif // BSP_H
