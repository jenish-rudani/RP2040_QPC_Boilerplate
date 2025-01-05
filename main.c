/********************************************************************************************
*                                                                                           *
*                                        main.cpp                                           *
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

#include "qpc.h"              // QP/C real-time embedded framework
#include "bsp.h"              // Board Support Package
#include "pico/binary_info.h"
#include "hardware/gpio.h"


//............................................................................
int main() {
    bi_decl(bi_program_description("This is a test binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    QF_init();                  // initialize the framework
    BSP_init();                // Initialize the BSP
    BSP_start();               // Start the AOs/Threads
    return QF_run();       // Run the QF application
}
