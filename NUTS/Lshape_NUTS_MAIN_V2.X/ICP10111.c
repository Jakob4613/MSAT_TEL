#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/i2c_host/i2c1.h"
#include "mcc_generated_files/i2c_host/i2c_host_interface.h"
#include "ICP10111.h"
#include <stdio.h>
#include <math.h>

// REGISTER ADDRESSES COPIED FROM THE DATASHEET
#define ICP10111_ADDRESS 0x63

#define LOW_POWER_TP 0x609C
#define NORMAL_TP 0x6825
#define LOW_NOISE_TP 0x70DF
#define ULTRA_LOW_NOISE_TP 0x7866
#define READ_ID_CMD 0xEFC8

#define SELECTED_MODE 0x70DF

#define TEMPERATURE 303.15             // K (constant in isothermal assumption)
#define GAS_CONSTANT 8.3144598         // J/(mol·K)
#define GRAVITY 9.7796                // m/s^2
#define MOLAR_MASS 0.0289644           // kg/mol


/**
 * Function for calculating the HAG.
 * @param pressure measured pressure
 * @param ref_pressure Reference pressure, the pressure at HAG=0
 * @return a double for the HAG.
 */
double pressure_to_altitude(double pressure, double ref_pressure) {
    if(pressure >= ref_pressure){
        return 0;
    }
    else{
        double factor = (GAS_CONSTANT * TEMPERATURE) / (MOLAR_MASS * GRAVITY);
        double altitude = -factor * log(pressure / ref_pressure);
        return altitude;
    }
}


/**
 * Find the icp id. If properly installed, an 8 is returned.
 * @return the icp id in a uint8 type.
 */
uint8_t retrieve_partid_ICP10111(void) {
    
    // Desired write and read lengths
    uint8_t writeLength;
    uint8_t readLength;

    // Write and read data buffers (buffersize is set for some overhead)
    uint8_t transmitData[10] = {};
    uint8_t readData[4] = {};

    uint8_t result;
  

    // Write to client device
    transmitData[0] = (READ_ID_CMD >> 8) & 0xFF;
    transmitData[1] = READ_ID_CMD & 0xFF;
    writeLength = 2; // 2 bytes of location address + 1 byte data

    if ( I2C1_Host.Write(ICP10111_ADDRESS, transmitData, writeLength)){                                                                                       
        while ( I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();            
        }

        if (  I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            // Write operation is successful
        }
        else{
            return 0;
        }
    }

    
    // Read from client device
    readLength = 3;
    if (I2C1_Host.Read(ICP10111_ADDRESS, readData, readLength)){                                                                                       
        while (I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();          
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            // Read operation is successful
        }
        else{
            return 0;
        }
    }
    
    result = 0b00111111 & readData[1];
    
    return result;
}

/*
bool retrieve_data_ICP10111(uint16_t cfg_A, uint16_t cfg_B, uint16_t cfg_C, uint16_t cfg_D, float *temperature_ret, float *pressure_ret){
    // Desired write and read lengths
    uint8_t writeLength;
    uint8_t readLength;

    // Write and read data buffers (buffersize is set for some overhead)
    uint8_t transmitData[10] = {};
    uint8_t readData[4] = {};

    // Write to client device
    transmitData[0] = (SELECTED_MODE >> 8) & 0xFF;
    transmitData[1] = SELECTED_MODE & 0xFF;
    writeLength = 10; // 2 bytes of location address + 1 byte data

    if ( I2C1_Host.Write(ICP10111_ADDRESS, transmitData, writeLength)){                                                                                       
        while ( I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();             
        }

        if (  I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            // Write operation is successful
        }
        else{
            //return false;
        }
    }
    

    // Read from client device
    readLength = 9; // 1 byte read
    if (I2C1_Host.Read(ICP10111_ADDRESS, readData, readLength)){                                                                                        
        while (I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();            
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            // Read operation is successful
        }
        else{
            // error fallback
        }
    }
    
    int16_t T_LSB;
    T_LSB = ((int16_t)readData[0] << 8) | readData[1];
    
    
    uint32_t p_LSB;
    p_LSB = ((uint32_t)readData[3] << 16) | ((uint32_t)readData[4] << 8) | (uint32_t)readData[6];
    
    

    float _pcal[3] = { 45000.0, 80000.0, 105000.0 };
    float _scal[4] = {(float)cfg_A, (float)cfg_B, (float)cfg_C, (float)cfg_D};
    float _lut_lower = 3.5 * 0x100000;
    float _lut_upper = 11.5 * 0x100000;
    float _quadr_factor = 1 / 16777216.0;
    float _offst_factor = 2048.0;
    float sensorOTP[4];
    sensorOTP[0] = 223;
    sensorOTP[1] = 525;
    sensorOTP[2] = 926;
    sensorOTP[3] = 4014;
    
    float temperature_out = -45.f + 175.f / 65536.f * T_LSB;

	// calculate pressure
	float t = (float)(T_LSB - 32768);
	float s1 = _lut_lower + (float)(_scal[0] * t * t) * _quadr_factor;
	float s2 = _offst_factor * _scal[3] + (float)(_scal[1] * t * t) * _quadr_factor;
	float s3 = _lut_upper + (float)(_scal[2] * t * t) * _quadr_factor;
	float c = (s1 * s2 * (_pcal[0] - _pcal[1]) +
			   s2 * s3 * (_pcal[1] - _pcal[2]) +
			   s3 * s1 * (_pcal[2] - _pcal[0])) /
			  (s3 * (_pcal[0] - _pcal[1]) +
			   s1 * (_pcal[1] - _pcal[2]) +
			   s2 * (_pcal[2] - _pcal[0]));
	float a = (_pcal[0] * s1 - _pcal[1] * s2 - (_pcal[1] - _pcal[0]) * c) / (s1 - s2);
	float b = (_pcal[0] - a) * (s1 + c);
	float pressure_out = a + b / (c + p_LSB);

    *temperature_ret = temperature_out;
    *pressure_ret = pressure_out;
    
    return true;
}
*/

bool retrieve_data_ICP10111(uint16_t cfg_A, uint16_t cfg_B, uint16_t cfg_C, uint16_t cfg_D, float *temperature_ret, float *pressure_ret){
    // Desired write and read lengths
    uint8_t writeLength;
    uint8_t readLength;

    // Write and read data buffers (buffersize is set for some overhead)
    uint8_t transmitData[10] = {};
    uint8_t readData[4] = {};

    // Read from client device
    readLength = 9; // 1 byte read
    if (I2C1_Host.Read(ICP10111_ADDRESS, readData, readLength)){                                                                                        
        while (I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();            
        }

        if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            // Read operation is successful
        }
        else{
            // error fallback
        }
    }
    
    int16_t T_LSB;
    T_LSB = ((int16_t)readData[0] << 8) | readData[1];
    
    
    uint32_t p_LSB;
    p_LSB = ((uint32_t)readData[3] << 16) | ((uint32_t)readData[4] << 8) | (uint32_t)readData[6];
    
    float _pcal[3] = { 45000.0, 80000.0, 105000.0 };
    float _scal[4] = {(float)cfg_A, (float)cfg_B, (float)cfg_C, (float)cfg_D};
    float _lut_lower = 3.5 * 0x100000;
    float _lut_upper = 11.5 * 0x100000;
    float _quadr_factor = 1 / 16777216.0;
    float _offst_factor = 2048.0;
    float sensorOTP[4];
    sensorOTP[0] = 223;
    sensorOTP[1] = 525;
    sensorOTP[2] = 926;
    sensorOTP[3] = 4014;
    
    float temperature_out = -45.f + 175.f / 65536.f * T_LSB;

	// calculate pressure
	float t = (float)(T_LSB - 32768);
	float s1 = _lut_lower + (float)(_scal[0] * t * t) * _quadr_factor;
	float s2 = _offst_factor * _scal[3] + (float)(_scal[1] * t * t) * _quadr_factor;
	float s3 = _lut_upper + (float)(_scal[2] * t * t) * _quadr_factor;
	float c = (s1 * s2 * (_pcal[0] - _pcal[1]) +
			   s2 * s3 * (_pcal[1] - _pcal[2]) +
			   s3 * s1 * (_pcal[2] - _pcal[0])) /
			  (s3 * (_pcal[0] - _pcal[1]) +
			   s1 * (_pcal[1] - _pcal[2]) +
			   s2 * (_pcal[2] - _pcal[0]));
	float a = (_pcal[0] * s1 - _pcal[1] * s2 - (_pcal[1] - _pcal[0]) * c) / (s1 - s2);
	float b = (_pcal[0] - a) * (s1 + c);
	float pressure_out = a + b / (c + p_LSB);

    *temperature_ret = temperature_out;
    *pressure_ret = pressure_out;
    
    return true;
}

bool activate_ICP10111(void){
    
    // Desired write and read lengths
    uint8_t writeLength;

    // Write and read data buffers (buffersize is set for some overhead)
    uint8_t transmitData[10] = {};
    
    // Write to client device
    transmitData[0] = (SELECTED_MODE >> 8) & 0xFF;
    transmitData[1] = SELECTED_MODE & 0xFF;
    writeLength = 10; // 2 bytes of location address + 1 byte data
    
    //WRite command for reading new value.
    if ( I2C1_Host.Write(ICP10111_ADDRESS, transmitData, writeLength)){                                                                                       
        while ( I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();             
        }

        if (  I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            // Write operation is successful
        }
        else{
            //return false;
        }
    }
    
    return true;
}


bool initialize_ICP10111(uint16_t *a, uint16_t *b, uint16_t *c, uint16_t *d){
    // Desired write and read lengths
    uint8_t writeLength;
    uint8_t readLength;

    // Write and read data buffers (buffersize is set for some overhead)
    uint8_t transmitData[10] = {};
    uint8_t readData[4] = {};
  
    uint16_t abcd[4] = {};

    // Write to client device
    transmitData[0] = 0xC5;
    transmitData[1] = 0x95;
    transmitData[2] = 0x00;
    transmitData[3] = 0x66;
    transmitData[4] = 0x9C;
    writeLength = 5; // 2 bytes of location address + 1 byte data

    if ( I2C1_Host.Write(ICP10111_ADDRESS, transmitData, writeLength)){                                                                                       
        while ( I2C1_Host.IsBusy()){
            I2C1_Host.Tasks();          
        }

        if (  I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
            // Write operation is successful
        }
        else
        {
            return false;
        }
    }
    
    for(int i=0; i<4; i++){
        uint8_t transmitData2[2] = {};
        uint8_t writeLength2;
        
        transmitData2[0] = 0xC7;
        transmitData2[1] = 0xF7;
        writeLength2 = 2; // 2 bytes of location address + 1 byte data

        if ( I2C1_Host.Write(ICP10111_ADDRESS, transmitData2, writeLength2)){                                                                                    
            while ( I2C1_Host.IsBusy()){
                I2C1_Host.Tasks();             
            }

            if (  I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
                // Write operation is successful
            }
            else{
                return false;
            }
        }

   
        // Read from client device
        readLength = 3; // 1 byte read
        if (I2C1_Host.Read(ICP10111_ADDRESS, readData, readLength)){                                                                                       
            while (I2C1_Host.IsBusy()){
                I2C1_Host.Tasks();             
            }

            if ( I2C1_Host.ErrorGet() == I2C_ERROR_NONE){
                // Read operation is successful
            }
            else{
                return false;
            }
        }
        
        abcd[i] = ((uint16_t)readData[0] << 8) | readData[1];
    }
    
    *a = abcd[0];
    *b = abcd[1];
    *c = abcd[2];
    *d = abcd[3];
    return true;
}
