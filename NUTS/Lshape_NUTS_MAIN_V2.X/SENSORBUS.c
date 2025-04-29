#include "mcc_generated_files/system/system.h"
#include <stdbool.h>
#include <stdint.h>
#include "SENSORBUS.h"
#include <stdlib.h> 


/**
 * Function for writing an i2c byte to a slave.
 * @param slave_address The address of the slave.
 * @param register_address the register of the slave that needs to be changed.
 * @param to_write_byte the byte that needs to be written at the specific address.
 * @return a boolean indicating the succes of the writing; true = succes, false =
 * failure.
 */
bool write_IIC_byte(uint16_t slave_address, uint16_t register_address, char to_write_byte){

    // Define register address and data to be written
    uint8_t TxData[2] = {(uint8_t)register_address, (uint8_t)to_write_byte};
    // Write 2 bytes: register address + data byte
    
    if(I2C1_Write(slave_address, &TxData[0], 2)){                                                                                       
        while (I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();             
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

/**
 * Read an IIC byte from a slave. Often works for sensors attached to the PIC.
 * @param slave_address indicates the address of the slave device you want to  read from
 * @param register_address The register of the slave that you want to read
 * @param amount_of_bytes The amount of bytes you want to read. Currently only 1 or 2 is accepted.
 * @param LHorder If yuo read 2 bytes, select the order of reconstruction LH or HL. 
 * @return a uint16_t indicating the value of the read byte.
 */
uint16_t read_IIC_byte(uint16_t slave_address, uint16_t register_address, size_t amount_of_bytes, bool LHorder){

    // Define the register address and the data array
    uint8_t TxData[1] = {(uint8_t)register_address};  // Register address to read from
    
    uint8_t RxData[2];

    if(I2C1_Write(slave_address, &TxData[0], 1)){                                                                                   
        while (I2C1_Host.IsBusy())
        {
            I2C1_Host.Tasks();            
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE)
        {
            // Read operation is successful
        }
        else
        {
            // error fallback
        }
    }


    // Check for any errors in the write transaction
    uint8_t Result = (uint8_t) I2C1_ErrorGet(); // 0 = no error, >0 = error
    if (Result != 0) {
        return 0;
    }

    // Step 2: Perform a repeated start and read the data from the device at address 0x52
    if (I2C1_WriteRead(slave_address, &TxData[0], 1, &RxData[0], amount_of_bytes)) { // Write the register address, then read the data                                                                                        
        while (I2C1_Host.IsBusy()) {
            I2C1_Host.Tasks();
        }

        if (I2C1_Host.ErrorGet() == I2C_ERROR_NONE) {
            // Read operation is successful
        } else {
            // error fallback
        }
    }


    // Check for any errors in the read transaction
    Result = (uint8_t)I2C1_ErrorGet();  // 0 = no error, >0 = error
    if (Result == 0) {
        // Successfully read data, RxData[0] contains the value from register 0x20
        
        if (amount_of_bytes == 1){
            return RxData[0];
        }
        else{
            if(LHorder){
                return ((uint16_t)RxData[0] << 8) | RxData[1];
            }
            else{
                return ((uint16_t)RxData[1] << 8) | RxData[0];
            }
        }  
        
    } 
    else {
        return 0;
    }
}



/**
 * Read an IIC byte from a slave. Often works for sensors attached to the PIC.
 * @param slave_address indicates the address of the slave device you want to  read from
 * @param register_address The register of the slave that you want to read
 * @param LHorder If yuo read 2 bytes, select the order of reconstruction LH or HL. 
 * @return a uint16_t indicating the value of the read byte.
 */
bool read_double_IIC_byte(uint16_t slave_address, uint16_t register_address, uint16_t *read_value, bool LHorder){

    // Define the register address and the data array
    uint8_t TxData[1] = {(uint8_t)register_address};    // Register address to read from
    uint8_t RxData[2];                                  // Array to store the received data

    
    if(I2C1_Write(slave_address, &TxData[0], 1)){                                                                                   
        while (I2C1_Host.IsBusy())
        {
            I2C1_Host.Tasks();            
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE)
        {
            // Read operation is successful
        }
        else
        {
            // error fallback
        }
    }
    
    // Step 2: Perform a repeated start and read the data from the device at address 0x52
    if(I2C1_WriteRead(slave_address, &TxData[0], 1, &RxData[0], 2)){ // Write the register address, then read the data                                                                                      
        while (I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();  
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            if(LHorder){
                *read_value = ((uint16_t)RxData[0] << 8) | RxData[1];
            }
            else{
                *read_value = ((uint16_t)RxData[1] << 8) | RxData[0];
            }
            return true;
        }
        else{
            //return false;
        }
    }
    return true;

}




/**
 * Read an IIC byte from a slave. Often works for sensors attached to the PIC.
 * @param slave_address indicates the address of the slave device you want to  read from
 * @param register_address The register of the slave that you want to read
 * @return a uint16_t indicating the value of the read byte.
 */
bool read_single_IIC_byte(uint16_t slave_address, uint16_t register_address, uint8_t *read_val){

    // Define the register address and the data array
    uint8_t TxData[1] = {(uint8_t)register_address};  // Register address to read from
    uint8_t RxData; // Array to store the received data
        
    if(I2C1_Write(slave_address, &TxData[0], 1)){                                                                                   
        while (I2C1_Host.IsBusy())
        {
            I2C1_Host.Tasks();            
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE)
        {
            // Read operation is successful
        }
        else
        {
            // error fallback
        }
    }
    
    
    // Step 2: Perform a repeated start and read the data from the device at address 0x52
    if(I2C1_WriteRead(slave_address, &TxData[0], 1, &RxData, 1)){ // Write the register address, then read the data                                                                                      
        while (I2C1_Host.IsBusy())
        {
            I2C1_Host.Tasks();            
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE)
        {
            *read_val = RxData;
            return true;
        }
        else
        {
            //return false;
        }
    }
    //return false;
}
