#include "mcc_generated_files/system/system.h"
#include <stdbool.h>
#include <stdint.h>
#include "mcc_generated_files/i2c_host/i2c1.h"
#include "mcc_generated_files/i2c_host/i2c_host_interface.h"
#include "SENSORBUS.h"
#include "ENS160.h"


#define ENS160_ADDRESS 0x52
#define PART_ID_REG 0x00
#define OPMODE_REG 0x10
#define DATA_AQI_REG 0x21
#define DATA_ECO2_REG 0x24
#define DATA_TVOC_REG 0x22
#define DEVICE_STATUS_REG 0x20

/**
 * Initialize the ENS160 sensor. This includes setting the operational mode to 
 * gas sensing mode.
 * @return boolean indicating wether writing the i2c register was a success.
 */
bool initialize_ENS160(void)
{
    uint8_t buffer[2];

    buffer[0] = OPMODE_REG;  // Register address
    buffer[1] = 0x02;        // STANDARD mode

    // Try to start I2C write
    if (I2C1_Write(ENS160_ADDRESS, buffer, sizeof(buffer)))
    {
        // Wait for transfer to complete
        while (I2C1_Host.IsBusy())
        {
            I2C1_Host.Tasks();            
        }

        // Check for I2C transfer success
        if (I2C1_Host.ErrorGet() == I2C_ERROR_NONE)
        {
            return true;  // Success
        }
    }

    return false; // Failure
}

/**
 * Function for checking what the part-id of the ENS160 is. The function should return
 * 0x60 and 0x01 (352 in decimal) if the sensor is correctly set up.
 * @return the part-id as a uint16_t.
 */
uint16_t retrieve_partid_ENS160(void){
    uint16_t result;
    uint8_t reg = PART_ID_REG;
    uint8_t buffer[2] = {0};

    // Write register address
    if (!I2C1_Write(ENS160_ADDRESS, &reg, 1))
        return false;

    // Wait for write to complete
    while (I2C1_Host.IsBusy())
        I2C1_Host.Tasks();

    if (I2C1_Host.ErrorGet() != I2C_ERROR_NONE)
        return false;

    // Now Read 2 bytes
    if (!I2C1_Read(ENS160_ADDRESS, buffer, 2))
        return false;

    while (I2C1_Host.IsBusy())
        I2C1_Host.Tasks();

    if (I2C1_Host.ErrorGet() != I2C_ERROR_NONE)
        return false;

    result = (uint16_t)buffer[0] | ((uint16_t)buffer[1] << 8);

    return result;
}


/**
 * Retrieve a Co2 ppm measurement from the ENS160 sensor.
 * @return a uint16_t integer that describes the amount of CO2 in the air, in a 
 * PPM unit.
 */
bool retrieve_CO2_ENS160(uint16_t *eco2_ppm){
    uint8_t reg = DATA_ECO2_REG;
    uint8_t buffer[2] = {0};

    // Write register address
    if (!I2C1_Write(ENS160_ADDRESS, &reg, 1))
        return false;

    // Wait for write to complete
    while (I2C1_Host.IsBusy())
        I2C1_Host.Tasks();

    if (I2C1_Host.ErrorGet() != I2C_ERROR_NONE)
        return false;

    // Now Read 2 bytes
    if (!I2C1_Read(ENS160_ADDRESS, buffer, 2))
        return false;

    while (I2C1_Host.IsBusy())
        I2C1_Host.Tasks();

    if (I2C1_Host.ErrorGet() != I2C_ERROR_NONE)
        return false;

    *eco2_ppm = (uint16_t)buffer[0] | ((uint16_t)buffer[1] << 8);
    return true;
}



/**
 * Retrieve a TVOC in ppb measurement from the ENS160 sensor.
 * @return the total amount of TVOC in the air.
 */
bool retrieve_TVOC_ENS160(uint16_t *tvocppb){
    uint8_t reg = DATA_TVOC_REG;
    uint8_t buffer[2] = {0};

    // Write register address
    if (!I2C1_Write(ENS160_ADDRESS, &reg, 1))
        return false;

    // Wait for write to complete
    while (I2C1_Host.IsBusy())
        I2C1_Host.Tasks();

    if (I2C1_Host.ErrorGet() != I2C_ERROR_NONE)
        return false;

    // Now Read 2 bytes
    if (!I2C1_Read(ENS160_ADDRESS, buffer, 2))
        return false;

    while (I2C1_Host.IsBusy())
        I2C1_Host.Tasks();

    if (I2C1_Host.ErrorGet() != I2C_ERROR_NONE)
        return false;

    *tvocppb = (uint16_t)buffer[0] | ((uint16_t)buffer[1] << 8);
    return true;
}

/**
 * Retrieve the AQI measurement from the ENS160.
 * @return an integer between 1 and 5 describing air quality. 10 if iic failure.
 */
uint8_t retrieve_AQI_ENS160(void){
    uint8_t result;
    uint8_t reg = DATA_AQI_REG;
    uint8_t buffer[1] = {0};

    // Write register address
    if (!I2C1_Write(ENS160_ADDRESS, &reg, 1))
        return false;

    // Wait for write to complete
    while (I2C1_Host.IsBusy())
        I2C1_Host.Tasks();

    if (I2C1_Host.ErrorGet() != I2C_ERROR_NONE)
        return false;

    // Now Read 2 bytes
    if (!I2C1_Read(ENS160_ADDRESS, buffer, 1))
        return false;

    while (I2C1_Host.IsBusy())
        I2C1_Host.Tasks();

    if (I2C1_Host.ErrorGet() != I2C_ERROR_NONE)
        return false;

    result = buffer[0];

    return result;
}
