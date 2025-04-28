#ifndef EEPROM_FUNCTIONS_H
#define EEPROM_FUNCTIONS_H

bool set_reference_pressure(float ref_press);
bool set_global_state(uint8_t state);
float get_reference_pressure(void);
uint8_t get_global_state(void);

#endif