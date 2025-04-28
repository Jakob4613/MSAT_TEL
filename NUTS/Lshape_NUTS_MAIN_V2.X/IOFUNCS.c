#include "mcc_generated_files/system/system.h"
#include <stdbool.h>
#include <stdint.h>
#include "IOFUNCS.h"

/**
 * Function for setting up the IO capabilities for the chip.
 * Led1, led2 and bw activation are set as outputs. The battery measurement,
 * and burnwire measurement are set as analog inputs.
 */
void initialize_iopins(void){
    TRISCbits.TRISC7 = 0;      //Buzzer
    TRISAbits.TRISA2 = 0;      //BW activation
    
    // Configure Analog input.
    TRISBbits.TRISB7 = 1;       // Battery measurement
    ANSELBbits.ANSELB7 = 1;     // batt
    
    TRISCbits.TRISC3 = 1;       // Burnwire measurement
    ANSELCbits.ANSELC3 = 1;     // bw
    ADCON0bits.ON = 1;
}

/**
 * Function for reading the battery voltage from the ADC.
 * @return a uint16_t indicating the ADC output.
 */
uint16_t read_battery_voltage(void){
    ADPCHbits.PCH = 0b001111;
    ADCON0bits.GO=1; //Start the ADC conversion
    while(ADCON0bits.GO == 1); //Do nothing until the conversion has finished
    uint16_t result = (uint16_t)ADRESH << 8 | (uint16_t)ADRESL;
    return result;
}

/**
 * Function for reading the burnwire voltage from the ADC.
 * @return a uint16_t indicating the ADC's output.
 */
uint16_t read_brnwr_voltage(void){
    ADPCHbits.PCH = 0b010011;
    ADCON0bits.GO=1; //Start the ADC conversion
    while(ADCON0bits.GO == 1); //Do nothing until the conversion has finished
    uint16_t result = (uint16_t)ADRESH << 8 | (uint16_t)ADRESL;
    return result;
}

/**
 * Function for interfacing with the onboard_buzzer.
 * @param on_or_off boolean value indicating wether to turn the buzzer on or off. 
 * true = on, false = off.
 */
void activate_buzzer(bool on_or_off){
    LATCbits.LATC7 = on_or_off;
}

/**
 * Function for activating the burnwire.
 */
void activate_brnwr(bool on_or_off){
    LATAbits.LATA2 = on_or_off;
}