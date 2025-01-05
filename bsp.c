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

#include "qpc.h"              // QP/C framework API
#include "app.h"         // Pico-App Application interface
#include "bsp.h"              // Board Support Package interface
#include "RP2040.h"           // The device specific header
#include <stdio.h>            // for printf()
#include <stdlib.h>           // for exit()
#include "hardware/uart.h"    // for uart_init()

#ifndef PICO_DEFAULT_LED_PIN
    #error This application requires a board with a regular LED
#endif

// Local-scope objects -------------------------------------------------------
#ifdef Q_SPY
#ifdef Q_SPY
#error The Simple Application does not support Spy build configuration
#endif
// for future use
    static uart_inst_t *qs_uart = uart0;

    // QSpy source IDs
    static QSpyId const l_SysTick_Handler = { 0U };

    static QSTimeCtr QS_tickTime_;
    static QSTimeCtr QS_tickPeriod_;
#endif


//============================================================================
// Error handler and ISRs...

//............................................................................
Q_NORETURN Q_onError(char const * const module, int id) {
    //exit(-1);
    // NOTE: this implementation of the error handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U);
#ifndef NDEBUG
    // Light up the on-board LED
    gpio_put(LED_PIN, 1);
    // For debugging, hang on in an endless loop...
    for (;;) {
    }
#else
    NVIC_SystemReset();
    for (;;) { // explicitly "no-return"
    }
#endif
}
//............................................................................
void assert_failed(char const * const module, int_t const id);  // Prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

// ISRs used in the application ==============================================

void SysTick_Handler(void); // prototype
void SysTick_Handler()
{
    QK_ISR_ENTRY();

    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler); // time events at rate 0

#ifdef Q_SPY
    QS_tickTime_ += QS_tickPeriod_;     // Account for the clock rollover
#endif

    QK_ISR_EXIT();
}

#ifdef Q_SPY
// ISR for receiving bytes from the QSPY Back-End
// NOTE: This ISR is "QF-unaware" meaning that it does not interact with
// the QF/QK and is not disabled. Such ISRs don't need to call
// QK_ISR_ENTRY/QK_ISR_EXIT and they cannot post or publish events.

// TBD: UART ISR for QS-RX channel...
#endif // Q_SPY



//============================================================================
// BSP functions...
void BSP_init() {

    MPU->RBAR = 0x0U                          // base address (NULL)
            | MPU_RBAR_VALID_Msk          // valid region
            | (MPU_RBAR_REGION_Msk & 7U); // region #7
    MPU->RASR = (7U << MPU_RASR_SIZE_Pos)     // 2^(7+1) region
                | (0x0U << MPU_RASR_AP_Pos)   // no-access region
                | MPU_RASR_ENABLE_Msk;        // region enable
    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk       // enable background region
                | MPU_CTRL_ENABLE_Msk;        // enable the MPU
    __ISB();
    __DSB();

    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    stdio_init_all();                       // Activate if necessary
    // uart_init(qs_uart, 115200);             // Activate if necessary

    // uart_set_baudrate(uart0, 115200);
    // uart_set_hw_flow(qs_uart, false, false);
    // uart_set_format(qs_uart, 8, 1, UART_PARITY_NONE);
    // uart_set_fifo_enabled(qs_uart, false);

    //uart_init(qs_uart, 115200);             // Activate if necessary
    //uart_set_fifo_enabled(qs_uart, true);   // Enable UART FIFOs

    // Initialize the LED pin
    gpio_init(LED_PIN);                     // TODO: implement a "heartbeat" indicator
    gpio_set_dir(LED_PIN, GPIO_OUT);

#ifdef Q_SPY
    // initialize the QS Software Tracing...
    if (!QS_INIT(nullptr)) {
        Q_ERROR();
    }

    // Dictionaries...
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);

    QS_ONLY(produce_sig_dict());

    // Set up the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS);      // All records
    QS_GLB_FILTER(-QS_QF_TICK);         // Exclude the clock tick
#endif //Q_SPY
}

//............................................................................
void BSP_start() {
    
    // initialize event pools
    static QF_MPOOL_EL(QEvt) smlPoolSto[10];
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // initialize publish-subscribe
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // Instantiate and start the active objects/threads
    static QEvt const *appQueueSto[10];
    Blinky_ctor();
    QActive_start(AO_App,
        1U,                          // QP priority of the AO
        appQueueSto,                 // event queue storage
        Q_DIM(appQueueSto),          // queue length [events]
        (void *)0, 0U,               // no stack storage
        (void *)0);                  // no initialization param
}

//............................................................................
void BSP_ledOn() {
    gpio_put(LED_PIN, 1);
}

//............................................................................
void BSP_ledOff() {
    gpio_put(LED_PIN, 0);
}

//............................................................................
void BSP_terminate(int16_t result) {
    Q_UNUSED_PAR(result);
}

//============================================================================
uint BSP_TICKS_PER_SEC = 1000;

// QF callbacks...
void QF_onStartup() {
    // Set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC - 1);
}

//............................................................................
void QF_onCleanup(void) {
#ifdef Q_SPY
    QS_rxParse();  // Parse all received bytes

    if (uart_is_writable(qs_uart)) {
        QF_INT_DISABLE();
        uint16_t fifo = 32; // max bytes we can accept
        uint16_t b = QS_getBlock(&fifo); //TODO: Update with Block link: https://sourceforge.net/p/qpc/discussion/668726/thread/64ded00380/?limit=25
        QF_INT_ENABLE();

        if (b != QS_EOD) {                  // Not End-Of-Data?
            uart_putc_raw(qs_uart, (char) b);      // Send character
            printf("%c", (char) b);
        }
    }
#endif
}

//............................................................................
void QK_onIdle(void) {
#ifdef Q_SPY
    QS_rxParse();  // Parse all received bytes

    if (uart_is_writable(qs_uart)) {
        QF_INT_DISABLE();
        uint16_t fifo = 32; // max bytes we can accept
        uint16_t b = QS_getBlock(&fifo); //TODO: Update with Block link: https://sourceforge.net/p/qpc/discussion/668726/thread/64ded00380/?limit=25
        QF_INT_ENABLE();

        if (b != QS_EOD) {                  // Not End-Of-Data?
            uart_putc_raw(qs_uart, (char) b);      // Send character
            printf("%c", (char) b);
        }
    }
#endif
}

//============================================================================
// QS callbacks...
#ifdef Q_SPY//............................................................................
bool onStartup(void const *arg) {
    Q_UNUSED_PAR(arg);

    static uint8_t qsTxBuf[2*1024];    // Buffer for QS transmit channel
    initBuf(qsTxBuf, sizeof(qsTxBuf));

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_;          // To start the timestamp at zero

    return true;    // Return success
}
//............................................................................
void onCleanup() {
}
//............................................................................
QSTimeCtr onGetTime() { // NOTE: invoked with interrupts DISABLED
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0U) { // not set?
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else { // the rollover occurred, but the SysTick_ISR did not run yet
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
//............................................................................
// NOTE:
// No critical section in onFlush() to avoid nesting of critical sections
// in case onFlush() is called from Q_onError().
void onFlush() {
    for (;;) {
        uint16_t b = getByte();
        if (b != QS_EOD) {
            uart_tx_wait_blocking(qs_uart);     // Wait for TX queue empty
            //uart_putc_raw(qs_uart, (char) b);   // Send character
            uart_putc(qs_uart, (char) b);       // Send character
        }
        else {
            break;
        }
    }
}
//............................................................................
void onReset() {
    NVIC_SystemReset();
}
//............................................................................
void onCommand(uint8_t cmdId, uint32_t param1,
               uint32_t param2, uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}
#endif // Q_SPY
//----------------------------------------------------------------------------

