#include "mcc_generated_files/system/system.h"
#include <stdio.h>
#include <string.h>
#include <xc.h>
#include "NRF24L01.h"

#define CONFIG_REG 0x00
#define SETUP_RETR_REG 0x04
#define RF_CH_REG 0x05
#define RF_SETUP_REG 0x06
#define SETUP_AW_REG 0x03
#define EN_AA_REG 0x01
#define FEATURE_REG 0x1D

#define W_TX_PAYLOAD_NOACK_CMD 0b10110000
#define W_TX_PAYLOAD_CMD 0b10100000
#define NOP_CMD 0xFF
#define FLUSH_TX_CMD 0b11100001


#define CONFIG_RF_SETUP 0b00100111 // 0dbm, 250kps
//#define CONFIG_RF_SETUP 0b00100101; // -6dbm, 250kps
//#define CONFIG_RF_SETUP 0b00100011; // -12dbm, 250kps
//#define CONFIG_RF_SETUP 0b00100001; // -18dbm, 250kps

#define CONFIG_CONFIG 0b00001010    //set the CONFIG byte such that no interrupts are triggerd, CRC isenabled and set to 1 byte
                                    // additionally set power up and to PTX.
#define CONFIG_FEATURE 0b00000001   //Turn on the W_TX_PAYLOAD_NOACK command!
#define CONFIG_ENAA 0x00            // Disable the auto acknowledgement on all pipes.
#define CONFIG_CH_REG 0x28          // hex28 is gelijk aan 40. The frequency is determined by F0 = 2400 + CH_REG [Mhz].
//#define CONFIG_SETUPAW 0b00000011   // 5 bit channel width
#define CONFIG_SETUPAW 0b00000001   // 3 bit channel width
#define CONFIG_RETR_REG 0x00        // disable auto-retransmit


/**
 * Read a single register from the NRF chip.
 * @param registerAddress The register that you want to read
 * @return The value of the read byte.
 */
uint8_t read_register_NRF(uint8_t registerAddress) {
    uint8_t result;
    SPI1_Open(SPI1_DEFAULT);
    NRF_SLAVE_SetLow();
    SPI1_ByteExchange(registerAddress);
    result = SPI1_ByteExchange(0xFF);
    NRF_SLAVE_SetHigh();
    SPI1_Close();
    return result;
}


void write_register_NRF(uint8_t registerAddress, uint8_t* dataToWrite, size_t dataSize) {
    uint8_t command = (0b00100000 | registerAddress);  // Write command with register address
    
    SPI1_Open(SPI1_DEFAULT);
    NRF_SLAVE_SetLow();
    
    SPI1_ByteExchange(command);  // Send Write command
    for (size_t i = 0; i < dataSize; i++) {
        SPI1_ByteExchange(dataToWrite[i]);  // Send data bytes
    }
    
    NRF_SLAVE_SetHigh();
    SPI1_Close();
}


void sendW_TX_PAYLOAD_NOACK(uint8_t* txPayloadData, size_t dataSize){
    uint8_t command[33];  // Command word + data bytes
    command[0] = W_TX_PAYLOAD_NOACK_CMD;   // W_TX_PAYLOAD command
    
    // Copy the payload data to the command buffer
    memcpy(&command[1], txPayloadData, dataSize);
    
    SPI1_Open(SPI1_DEFAULT);
    NRF_SLAVE_SetLow();
    
    // Clear TX buffer
    SPI1_ByteExchange(FLUSH_TX_CMD);
    
    NRF_SLAVE_SetHigh();
    __delay_ms(1);
    NRF_SLAVE_SetLow();
    
    
    for (int i = 0; i < dataSize+1; i++){ // + 1 for the command byte
        while(!SPI1_IsTxReady());
        SPI1_ByteExchange(command[i]);
    }
    
    NRF_SLAVE_SetHigh();
    SPI1_Close();
}


/**
 * Initialize the NRF24 with the preconfigured settings encoded in this function.
 * @return 
 */
bool initialize_NRF24L01(void){
        
    bool success = true;
    uint8_t rf_setup_value = CONFIG_RF_SETUP;
    write_register_NRF(RF_SETUP_REG, &rf_setup_value, 1);
    if(!(read_register_NRF(RF_SETUP_REG) == rf_setup_value)){
        success = false;
    }
     
    
    uint8_t config_value = CONFIG_CONFIG;
    write_register_NRF(CONFIG_REG, &config_value, 1);
    
    if(!(read_register_NRF(CONFIG_REG) == config_value)){
        success = false;
    }
    
    uint8_t feature_value = CONFIG_FEATURE;
    write_register_NRF(FEATURE_REG, &feature_value, 1);
    
    if(!(read_register_NRF(FEATURE_REG) == feature_value)){
        success = false;
    }    
    
    uint8_t enaa_value = CONFIG_ENAA;
    write_register_NRF(EN_AA_REG, &enaa_value, 1);
    
    if(!(read_register_NRF(EN_AA_REG) == enaa_value)){
        success = false;
    }      
    
    // Set the channel of the RF comms
    uint8_t rf_ch_reg_value = CONFIG_CH_REG;
    write_register_NRF(RF_CH_REG, &rf_ch_reg_value, 1);
    
    if(!(read_register_NRF(RF_CH_REG) == rf_ch_reg_value)){
        success = false;
    }
    
    
    uint8_t setupaw_value = CONFIG_SETUPAW;
    write_register_NRF(SETUP_AW_REG, &setupaw_value, 1);
    
    if(!(read_register_NRF(SETUP_AW_REG) == setupaw_value)){
        success = false;
    }
    
    
    uint8_t setup_retr_value = CONFIG_RETR_REG;
    write_register_NRF(SETUP_RETR_REG, &setup_retr_value, 1);
    
    if(!(read_register_NRF(SETUP_RETR_REG) == setup_retr_value)){
        success = false;
    }
    
    return success;
    
}


/**
 * Function for checking whether the RF 24 sends responds to the MOSI line.
 * Should return 14 if everything is connected as it should.
 * @return value of the status register.
 */
uint8_t retrieve_partid_NRF24L01(void) {
    uint8_t status;
    SPI1_Open(SPI1_DEFAULT);
    
    NRF_SLAVE_SetLow();
    status = SPI1_ByteExchange(0xAA);//NOP_CMD);  // Read STATUS register command
    NRF_SLAVE_SetHigh();
    SPI1_Close();
    
    return status;
}
