/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.1.1
*/

/*
� [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA2 aliases
#define BW_BLADESACT_TRIS                 TRISAbits.TRISA2
#define BW_BLADESACT_LAT                  LATAbits.LATA2
#define BW_BLADESACT_PORT                 PORTAbits.RA2
#define BW_BLADESACT_WPU                  WPUAbits.WPUA2
#define BW_BLADESACT_OD                   ODCONAbits.ODCA2
#define BW_BLADESACT_ANS                  ANSELAbits.ANSELA2
#define BW_BLADESACT_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define BW_BLADESACT_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define BW_BLADESACT_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define BW_BLADESACT_GetValue()           PORTAbits.RA2
#define BW_BLADESACT_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define BW_BLADESACT_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define BW_BLADESACT_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define BW_BLADESACT_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define BW_BLADESACT_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define BW_BLADESACT_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define BW_BLADESACT_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define BW_BLADESACT_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set RB5 aliases
#define IO_RB5_TRIS                 TRISBbits.TRISB5
#define IO_RB5_LAT                  LATBbits.LATB5
#define IO_RB5_PORT                 PORTBbits.RB5
#define IO_RB5_WPU                  WPUBbits.WPUB5
#define IO_RB5_OD                   ODCONBbits.ODCB5
#define IO_RB5_ANS                  ANSELBbits.
#define IO_RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define IO_RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define IO_RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define IO_RB5_GetValue()           PORTBbits.RB5
#define IO_RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define IO_RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define IO_RB5_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define IO_RB5_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define IO_RB5_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define IO_RB5_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define IO_RB5_SetAnalogMode()      do { ANSELBbits. = 1; } while(0)
#define IO_RB5_SetDigitalMode()     do { ANSELBbits. = 0; } while(0)

// get/set RB6 aliases
#define IO_RB6_TRIS                 TRISBbits.TRISB6
#define IO_RB6_LAT                  LATBbits.LATB6
#define IO_RB6_PORT                 PORTBbits.RB6
#define IO_RB6_WPU                  WPUBbits.WPUB6
#define IO_RB6_OD                   ODCONBbits.ODCB6
#define IO_RB6_ANS                  ANSELBbits.
#define IO_RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define IO_RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define IO_RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define IO_RB6_GetValue()           PORTBbits.RB6
#define IO_RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define IO_RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define IO_RB6_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define IO_RB6_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define IO_RB6_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define IO_RB6_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define IO_RB6_SetAnalogMode()      do { ANSELBbits. = 1; } while(0)
#define IO_RB6_SetDigitalMode()     do { ANSELBbits. = 0; } while(0)

// get/set RC0 aliases
#define IO_RC0_TRIS                 TRISCbits.TRISC0
#define IO_RC0_LAT                  LATCbits.LATC0
#define IO_RC0_PORT                 PORTCbits.RC0
#define IO_RC0_WPU                  WPUCbits.WPUC0
#define IO_RC0_OD                   ODCONCbits.ODCC0
#define IO_RC0_ANS                  ANSELCbits.
#define IO_RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define IO_RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define IO_RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define IO_RC0_GetValue()           PORTCbits.RC0
#define IO_RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define IO_RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define IO_RC0_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define IO_RC0_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define IO_RC0_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define IO_RC0_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define IO_RC0_SetAnalogMode()      do { ANSELCbits. = 1; } while(0)
#define IO_RC0_SetDigitalMode()     do { ANSELCbits. = 0; } while(0)

// get/set RC1 aliases
#define IO_RC1_TRIS                 TRISCbits.TRISC1
#define IO_RC1_LAT                  LATCbits.LATC1
#define IO_RC1_PORT                 PORTCbits.RC1
#define IO_RC1_WPU                  WPUCbits.WPUC1
#define IO_RC1_OD                   ODCONCbits.ODCC1
#define IO_RC1_ANS                  ANSELCbits.
#define IO_RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define IO_RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define IO_RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define IO_RC1_GetValue()           PORTCbits.RC1
#define IO_RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define IO_RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define IO_RC1_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define IO_RC1_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define IO_RC1_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define IO_RC1_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define IO_RC1_SetAnalogMode()      do { ANSELCbits. = 1; } while(0)
#define IO_RC1_SetDigitalMode()     do { ANSELCbits. = 0; } while(0)

// get/set RC4 aliases
#define IO_RC4_TRIS                 TRISCbits.TRISC4
#define IO_RC4_LAT                  LATCbits.LATC4
#define IO_RC4_PORT                 PORTCbits.RC4
#define IO_RC4_WPU                  WPUCbits.WPUC4
#define IO_RC4_OD                   ODCONCbits.ODCC4
#define IO_RC4_ANS                  ANSELCbits.ANSELC4
#define IO_RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define IO_RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define IO_RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define IO_RC4_GetValue()           PORTCbits.RC4
#define IO_RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define IO_RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define IO_RC4_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define IO_RC4_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define IO_RC4_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define IO_RC4_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define IO_RC4_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define IO_RC4_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set RC5 aliases
#define NRF_SLAVE_TRIS                 TRISCbits.TRISC5
#define NRF_SLAVE_LAT                  LATCbits.LATC5
#define NRF_SLAVE_PORT                 PORTCbits.RC5
#define NRF_SLAVE_WPU                  WPUCbits.WPUC5
#define NRF_SLAVE_OD                   ODCONCbits.ODCC5
#define NRF_SLAVE_ANS                  ANSELCbits.ANSELC5
#define NRF_SLAVE_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define NRF_SLAVE_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define NRF_SLAVE_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define NRF_SLAVE_GetValue()           PORTCbits.RC5
#define NRF_SLAVE_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define NRF_SLAVE_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define NRF_SLAVE_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define NRF_SLAVE_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define NRF_SLAVE_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define NRF_SLAVE_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define NRF_SLAVE_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define NRF_SLAVE_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RC6 aliases
#define CFG_IN_TRIS                 TRISCbits.TRISC6
#define CFG_IN_LAT                  LATCbits.LATC6
#define CFG_IN_PORT                 PORTCbits.RC6
#define CFG_IN_WPU                  WPUCbits.WPUC6
#define CFG_IN_OD                   ODCONCbits.ODCC6
#define CFG_IN_ANS                  ANSELCbits.ANSELC6
#define CFG_IN_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define CFG_IN_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define CFG_IN_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define CFG_IN_GetValue()           PORTCbits.RC6
#define CFG_IN_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define CFG_IN_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define CFG_IN_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define CFG_IN_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define CFG_IN_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define CFG_IN_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define CFG_IN_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define CFG_IN_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 aliases
#define BW_LEGS_ACT_TRIS                 TRISCbits.TRISC7
#define BW_LEGS_ACT_LAT                  LATCbits.LATC7
#define BW_LEGS_ACT_PORT                 PORTCbits.RC7
#define BW_LEGS_ACT_WPU                  WPUCbits.WPUC7
#define BW_LEGS_ACT_OD                   ODCONCbits.ODCC7
#define BW_LEGS_ACT_ANS                  ANSELCbits.ANSELC7
#define BW_LEGS_ACT_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define BW_LEGS_ACT_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define BW_LEGS_ACT_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define BW_LEGS_ACT_GetValue()           PORTCbits.RC7
#define BW_LEGS_ACT_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define BW_LEGS_ACT_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define BW_LEGS_ACT_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define BW_LEGS_ACT_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define BW_LEGS_ACT_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define BW_LEGS_ACT_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define BW_LEGS_ACT_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define BW_LEGS_ACT_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/