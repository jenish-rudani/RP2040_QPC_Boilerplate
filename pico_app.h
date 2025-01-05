/********************************************************************************************
*                                                                                           *
*                                          bsp.cpp                                          *
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

#ifndef APP_H
#define APP_H

enum AppSignals {
    SYS_TICK_SIG = Q_USER_SIG,
    TIMEOUT_SIG,
    MAX_PUB_SIG,
    MAX_SIG
};

void Blinky_ctor(void);
extern QActive * const AO_App; // opaque pointer

#ifdef Q_SPY
inline void produce_sig_dict() {
    QS_SIG_DICTIONARY(SYS_TICK_SIG, nullptr);
    QS_SIG_DICTIONARY(TIMEOUT_SIG,  nullptr);
}
#endif // def Q_SPY


#endif // APP_H
