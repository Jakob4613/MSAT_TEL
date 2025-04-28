#ifndef ICM_SENSEBUS_H
#define ICM_SENSEBUS_H

bool write_IIC_byte(uint16_t slave_address, uint16_t register_address, char to_write_byte);
uint16_t read_IIC_byte(uint16_t slave_address, uint16_t register_address, size_t amount_of_bytes, bool LHorder);
bool read_single_IIC_byte(uint16_t slave_address, uint16_t register_address, uint8_t *read_val);
bool read_double_IIC_byte(uint16_t slave_address, uint16_t register_address, uint16_t *read_value, bool LHorder);

#endif