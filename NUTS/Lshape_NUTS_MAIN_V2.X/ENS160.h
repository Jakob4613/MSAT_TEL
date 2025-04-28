#ifndef ENS_FUNCTIONS_H
#define ENS_FUNCTIONS_H

uint16_t retrieve_partid_ENS160(void);
bool initialize_ENS160(void);
bool retrieve_CO2_ENS160(uint16_t *co2ppm);
bool retrieve_TVOC_ENS160(uint16_t *tvocppb);
uint8_t retrieve_AQI_ENS160(void);
        
#endif