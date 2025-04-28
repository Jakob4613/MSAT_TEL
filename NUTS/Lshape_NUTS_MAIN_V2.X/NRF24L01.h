#ifndef NRF24L01_FUNCTIONS_H
#define NRF24L01_FUNCTIONS_H

bool initialize_NRF24L01(void);
uint8_t read_register_NRF(uint8_t registerAddress);
void write_register_NRF(uint8_t registerAddress, uint8_t* dataToWrite, size_t dataSize);
void sendW_TX_PAYLOAD_NOACK(uint8_t* txPayloadData, size_t dataSize);
uint8_t retrieve_partid_NRF24L01(void);

#endif