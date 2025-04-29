#ifndef ICP_FUNCTIONS_H
#define ICP_FUNCTIONS_H

uint8_t retrieve_partid_ICP10111(void);
double pressure_to_altitude(double pressure, double ref_pressure);
bool activate_ICP10111(void);
bool retrieve_data_ICP10111(uint16_t cfg_A, uint16_t cfg_B, uint16_t cfg_C, uint16_t cfg_D, float *temperature_ret, float *pressure_ret);
bool initialize_ICP10111(uint16_t *a, uint16_t *b, uint16_t *c, uint16_t *d);

#endif