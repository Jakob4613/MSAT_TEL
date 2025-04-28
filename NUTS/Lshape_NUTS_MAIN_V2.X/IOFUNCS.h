#ifndef IOFUNCS_FUNCTIONS_H
#define IOFUNCS_FUNCTIONS_H

void initialize_iopins(void);
uint16_t read_battery_voltage(void);
uint16_t read_brnwr_voltage(void);
void activate_buzzer(bool on_or_off);
void activate_brnwr(bool on_or_off);
bool read_config_button(void);

#endif