#ifndef ICM_FUNCTIONS_H
#define ICM_FUNCTIONS_H

uint8_t retrieve_partid_ICM20984(void);
bool retrieve_data_ICM20948(int16_t *acx, int16_t *acy, int16_t *acz, int16_t *gyx, int16_t *gyy, int16_t *gyz);
bool initialize_ICM20948(void);
bool retrieve_ERR_ICM20948(void);

#endif