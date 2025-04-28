#include "mcc_generated_files/system/system.h"
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>
#include "EEPROM_ACCESS.h"

// These are the EEPROM addresses in which the two variables are stored.
// Note that the ref press adress stores two bytes, thus expanding the range a bit.
#define REF_PRESS_ADDRESS 0x380003
#define GLOBAL_STATE_ADDRESS 0x380000

/**
 * Function used for storing the reference pressure. Ideally this function is only
 * called upon cansat initialization. Reference pressure is stored in EEPROM
 * to prevent losing the reference pressure upon satelite reset.
 * @param ref_press The reference pressure to be stored in EEPROM
 * @return boolean value indicating whether the reference pressure was succesfully
 * written.
 * 
 */
bool set_reference_pressure(float ref_press){
    bool success = true;

    // Use a union to access the individual bytes of the float
    union {
        float f;
        uint8_t bytes[4];
    } float_bytes;

    float_bytes.f = ref_press;

    for (uint8_t i = 0; i < 4; i++) {
        NVM_UnlockKeySet(UNLOCK_KEY);
        EEPROM_Write(REF_PRESS_ADDRESS + i, float_bytes.bytes[i]);
        NVM_UnlockKeyClear();

        while (NVM_IsBusy());

        if (NVM_StatusGet() != NVM_OK) {
            success = false;
            NVM_StatusClear();
        }
    }

    return success;
}

/**
 * Retrieve the reference pressure value as its stored in the EEPROM.
 * @return float containing the set up reference pressure.
 */
float get_reference_pressure(void){
    union {
        float f;
        uint8_t bytes[4];
    } float_bytes;

    float_bytes.bytes[0] = EEPROM_Read(REF_PRESS_ADDRESS);
    float_bytes.bytes[1] = EEPROM_Read(REF_PRESS_ADDRESS + 1);
    float_bytes.bytes[2] = EEPROM_Read(REF_PRESS_ADDRESS + 2);
    float_bytes.bytes[3] = EEPROM_Read(REF_PRESS_ADDRESS + 3);

    return float_bytes.f;
}

/**
 * Set the global state variable in the EEPROM. This makes the satellite keep 
 * track of its status independent of brownouts or mcu resets.
 * @param state The state that you want to set.
 * 0 = INIT,
 * 1 = LIFT,
 * 2 = DRP1,
 * 3 = BWA,
 * 4 = DRP2,
 * 5 = TDW
 * @return A boolean indicating whether writing to the EEPROM was succesfull. 
 */
bool set_global_state(uint8_t state){
    bool success = true;
    NVM_UnlockKeySet(UNLOCK_KEY);
    EEPROM_Write(GLOBAL_STATE_ADDRESS, state);
    NVM_UnlockKeyClear();
    while (NVM_IsBusy());
    if (!(NVM_StatusGet() == NVM_OK)){
        success = false;
        NVM_StatusClear();
    }
    
    return success;
}

/**
 * Function to retrieve the global state from the eeprom.
 * @return a uint8_t variable containing the global state variable.
 * 0 = INIT,
 * 1 = LIFT,
 * 2 = DRP1,
 * 3 = BWA,
 * 4 = DRP2,
 * 5 = TDW
 */
uint8_t get_global_state(void){
    uint8_t result;
    result = (uint8_t)EEPROM_Read(GLOBAL_STATE_ADDRESS);
    return result; 
}