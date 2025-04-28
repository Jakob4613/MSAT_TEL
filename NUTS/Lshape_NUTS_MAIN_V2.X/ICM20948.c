#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/i2c_host/i2c1.h"
#include "mcc_generated_files/i2c_host/i2c_host_interface.h"
#include "SENSORBUS.h"
#include "ICM20948.h"

// REGISTER ADDRESSES COPIED FROM THE DATASHEET
#define ICM20984_ADDRESS 0x68

#define WHO_AM_I_REG 0x00
#define PWR_MGMT_1_REG 0x06
#define ACCELX_OUT_REG 0x2D
#define ACCELY_OUT_REG 0x2F
#define ACCELZ_OUT_REG 0x31
#define GYROX_OUT_REG 0x33
#define GYROY_OUT_REG 0x35
#define GYROZ_OUT_REG 0x37
#define LP_CONFIG_REG 0x05
#define REG_BANK_SELECT_REG 0x7F
#define GYRO_CONFIG_1_REG 0x01
#define ACCEL_CONFIG_REG 0x14



/**
 * Function for checking what the part-id of the ICM20984 is. The function should return
 * 234 (decimal) if the sensor is correctly set up.
 * @return the part-id as a uint8_t.
 */
uint8_t retrieve_partid_ICM20984(void){
    return (uint8_t)read_IIC_byte(ICM20984_ADDRESS, WHO_AM_I_REG, 1, true);

//    uint8_t part_id;
//    if(read_single_IIC_byte(ICM20984_ADDRESS, WHO_AM_I_REG, &part_id)){
//        return part_id;
//    }
//    else{
//        return 0;
//    }
}


/**
 * Function for initializing the IMU. this means disabling the temperature sensor,
 * pulling the chip from sleep and setting the proper sensor sensitivities.
 * @return boolean value indicating the succes of initializing the IMU. returns 
 * true if everything worked out as expected.
 */
bool initialize_ICM20948(void){
    
    if (!write_IIC_byte(ICM20984_ADDRESS, PWR_MGMT_1_REG, 0x05)) {
        return false;
    }
    
    if (!write_IIC_byte(ICM20984_ADDRESS, LP_CONFIG_REG, 0x00)){
        return false;
    }
    
    // go to the user bank 2 to set the accel/gyro ranges.
    if (!write_IIC_byte(ICM20984_ADDRESS, REG_BANK_SELECT_REG, 0b00100000)){
        return false;
    }
    
    // Set the sensitivities, gyro set to +-2000 dps. ACcelerometer set to +- 16g.
    if (!write_IIC_byte(ICM20984_ADDRESS, GYRO_CONFIG_1_REG, 0x07)){
        return false;
    }
    if (!write_IIC_byte(ICM20984_ADDRESS, ACCEL_CONFIG_REG, 0x07)){
        return false;
    }

    // return to user bank 1 to retrieve accel/gyro values
    if (!write_IIC_byte(ICM20984_ADDRESS, REG_BANK_SELECT_REG, 0x00)){
        return false;
    }
    
    // If everything executed correctly:
    return true;
}

/**
 * TODO TODO TODO -> check for potential errors.
 * @return 
 */
bool retrieve_data_ICM20948(int16_t *acx, int16_t *acy, int16_t *acz, int16_t *gyx, int16_t *gyy, int16_t *gyz){
    *acx = (int16_t)read_IIC_byte(ICM20984_ADDRESS, ACCELX_OUT_REG, 2, true);
    *acy = (int16_t)read_IIC_byte(ICM20984_ADDRESS, ACCELY_OUT_REG, 2, true);
    *acz = (int16_t)read_IIC_byte(ICM20984_ADDRESS, ACCELZ_OUT_REG, 2, true);
    
    *gyx = (int16_t)read_IIC_byte(ICM20984_ADDRESS, GYROX_OUT_REG, 2, true);
    *gyy = (int16_t)read_IIC_byte(ICM20984_ADDRESS, GYROY_OUT_REG, 2, true);
    *gyz = (int16_t)read_IIC_byte(ICM20984_ADDRESS, GYROZ_OUT_REG, 2, true);
    
    return retrieve_ERR_ICM20948();

}


/**
 * Retrieve any errors possibly detectable by the ICM20948.
 * LARGE TODO!
 * @return boolean indicating whether an error was found. true = error, false = fine.
 */
bool retrieve_ERR_ICM20948(void){
    bool error_indicator = false;
    return error_indicator;
}