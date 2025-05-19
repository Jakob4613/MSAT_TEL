#include "mcc_generated_files/system/system.h"
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "SENSORBUS.h"
#include "ENS160.h"
#include "ICM20948.h"
#include "ICP10111.h"
#include "IOFUNCS.h"
#include "EEPROM_ACCESS.h"
#include "NRF24L01.h"

// DONT FORGET TO SET THE BARO_TIMER TO THE PROPER AMOUNT OF TIME!
#define LIFT_THRESHOLD 20 // If the satellite surpasses this value, the sat is able to go into DRP1
#define DRP1_THRESHOLD 250 //If the satellite drops below this value, the burnwire is capable of triggering
#define BWA_BLADES_THRESHOLD 200 // If the satellite surpasses this value, the burnwire is activated
#define BWA_LEGS_THRESHOLD 50 // If the satellite surpasses this value, the burnwire is activated
#define TDW_THRESHOLD 10 // If the satellite surpasses this value, the sat goes into touchdown-mode

struct {
    uint8_t COO_L: 8;
    uint8_t COO_H: 8;
    uint8_t APM_L: 8;
    uint8_t APM_H: 8;
    uint8_t ACX_L: 8;
    uint8_t ACX_H: 8;
    uint8_t ACY_L: 8;
    uint8_t ACY_H: 8;
    uint8_t ACZ_L: 8;
    uint8_t ACZ_H: 8;
    uint8_t GYX_L: 8;
    uint8_t GYX_H: 8;
    uint8_t GYY_L: 8;
    uint8_t GYY_H: 8;
    uint8_t GYZ_L: 8;
    uint8_t GYZ_H: 8;
    uint8_t TMP: 8;
    uint8_t BVO: 8;
    uint8_t CVO: 8;
    uint8_t SPD_L: 8;
    uint8_t SPD_H: 8;
    uint8_t UPT_L: 8;
    uint8_t UPT_H: 8;
    uint8_t HAG_L: 8;
    uint8_t HAG_H: 8;
    uint8_t APR_L: 8;
    uint8_t APR_H: 8;
    bool RTL: 1;
    bool RBR: 1;
    bool RAC: 1;
    bool RGS: 1;
    uint8_t NST: 4;
    uint8_t NULL1: 8;
    uint8_t NULL2: 8;
    uint8_t NULL3: 8;
    uint8_t NULL4: 8;
} telemetry_package;

//                                                                                             VALUE UPDATES?
uint16_t COO_var; // 16b -> 16b                                                                     XXXXX
float TMP_var; // 16b -> 16b Temperature is transferred as two 8-bit words
uint8_t TMP_tel;
float APM_var; // 32b -> 32b Pressure measurement is transferred as 4-8bit words
uint16_t APM_tel;
int16_t ACX_var; // 16b -> 16b Accelerations are stored as signed integers from 2 bytes.            XXXXX
int16_t ACY_var; // 16b -> 16b                                                                      XXXXX
int16_t ACZ_var; // 16b -> 16b                                                                      XXXXX
int16_t GYX_var; // 16b -> 16b                                                                      XXXXX
int16_t GYY_var; // 16b -> 16b                                                                      XXXXX
int16_t GYZ_var; // 16b -> 16b                                                                      XXXXX
uint16_t BVO_var; // 16b -> 8b                                                                      XXXXX
uint16_t CVO_var; // 16b -> 8b                                                                      XXXXX
uint16_t SPD_var; // 16b -> 9b      ALWAYS ABLE TO LEAVE OUT IF NOT REQUIRED!
uint8_t NST_var = 0; // 8b -> 4b Variable is only sent in a 4-byte length. 4 bits to be discarded       XXXXX
uint24_t UPT_var; // 16b -> 13b                                                                     XXXXX
uint16_t UPT_tel;
bool RGS_var; // 1b -> 1b                                                                           XXXXX
bool RAC_var; // 1b -> 1b                                                                           XXXXX
bool RBR_var; // 1b -> 1b
bool RTL_var; // 1b -> 1b
double HAG_var; // 16b -> 14b only 14 bits to be sent. 2 bits to be discarded.
uint16_t HAG_tel;
float APR_var; // 32b -> 14b                                                                     XXXXX
uint16_t APR_tel;

uint16_t ICP_A;
uint16_t ICP_B;
uint16_t ICP_C;
uint16_t ICP_D;

int main(void)
{
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptDisable(); 
    initialize_iopins();
    
    __delay_ms(100);
    
    while(true){
        activate_brnwr_blades(true);
        __delay_ms(5000);
        activate_brnwr_blades(false);
        
        activate_brnwr_legs(true);
        __delay_ms(5000);
        activate_brnwr_legs(false);
        
    }
    
    
    
    

    // Check wether the part-ids align with the expected value!
    if(retrieve_partid_ENS160() == 352){
//        printf("SUCCESS: Gas sensor shows valid device id.\r\n");
        RGS_var = false;
    }
    else{
        RGS_var = true;
    }
    if(retrieve_partid_ICM20984() == 234){
        RAC_var = false;
//        printf("SUCCESS: Accelerometer shows valid device id.\r\n");
    }
    else{
        RAC_var = true;
//        printf("ERROR: Accelerometer shows an invalid device id.\r\n");
    }
    if(retrieve_partid_ICP10111() == 8){
//        printf("SUCCESS: Barometer shows valid device id.\r\n");
        RBR_var = false;
    }
    else{
        RBR_var = true;
//        printf("ERROR: Accelerometer shows an invalid device id.\r\n");
    }
    
    if(retrieve_partid_NRF24L01() == 14){
        RTL_var = false;
//        printf("SUCCESS: NRF24 has been detected.\r\n");
    }
    else{
        RTL_var = true;
//        printf("ERROR: NRF24 has not been detected.\r\n");
    }
       
    initialize_ENS160();
    initialize_ICM20948();
    initialize_ICP10111(&ICP_A, &ICP_B, &ICP_C, &ICP_D);
    initialize_NRF24L01();
        
    // If the configuration button is pressed, these reference values need to be set.
    // Let the user know that the config button is pressed by beeping for 2 seconds.
    
    bool firstCheck = CFG_IN_GetValue();
    __delay_ms(50);
    if(CFG_IN_GetValue() == 0 && firstCheck == 0){
        activate_ICP10111();
        __delay_ms(100);
        retrieve_data_ICP10111(ICP_A, ICP_B, ICP_C, ICP_D, &TMP_var, &APM_var);
        set_reference_pressure(APM_var);
        APR_var = APM_var;
        set_global_state(0);

    }
    
    // If the configuration button is not pressed: the following occurs:
    // Let the user know with 5 short beeps that the MCU picks up from EEPROM.
    else{
        NST_var = get_global_state();
        APR_var = get_reference_pressure();
        UPT_var = 0;
    }
    
    activate_ICP10111();
    T2CONbits.TMR2ON = 1;
    __delay_ms(100);
        
    while(1){
        // 1. Retrieve the latest sensor-values. 
        // Write the accelerometer/gyroscope registers to the assigned variables. 
        // RAC stores whether an error occurred when reading the registers.
        retrieve_data_ICM20948(&ACX_var, &ACY_var, &ACZ_var, &GYX_var, &GYY_var, &GYZ_var);
        
        // Write the gas sensor register to the assigned variables.
        // RGS holds the gas sensor error indicated.
        retrieve_CO2_ENS160(&COO_var);
        
        
        //Check if the correct amount of time has elapsed prior to retrieving the new baro reading
        if(!T2CONbits.TMR2ON){
            retrieve_data_ICP10111(ICP_A, ICP_B, ICP_C, ICP_D, &TMP_var, &APM_var);
            activate_ICP10111();
            // Activate the timer that prevents reading ICP data too early!
            T2CONbits.TMR2ON = 1;
        }


        // Every state cycle, increase the uptime variable.
        UPT_var = UPT_var + 1;
                
        // Read the burn-wire and battery voltages into the designated variables.
        BVO_var = read_battery_voltage();
        CVO_var = read_brnwr_voltage();

        
        // 2/3. Determine the HAGL based on pressure sensor.
        HAG_var = pressure_to_altitude((double)APM_var, (double)APR_var);
        
        
//        printf("HEIGHT:%.2f\r\n\n\n\n", HAG_var);
//        printf("Uptime cycles:%d\r\n", UPT_var);
//        printf("Accelerometer X:%d\r\nAccelerometer Y:%d\r\nAccelerometer Z:%d\r\nGyroscope X:%d\r\nGyroscope Y:%d\r\nGyroscope Z:%d\r\n", ACX_var, ACY_var, ACZ_var, GYX_var, GYY_var, GYZ_var);
//        printf("Battery ADC:%u\r\nburnwire ADC:%u\r\n", BVO_var, CVO_var);
//        printf("Co2 contents:%d\r\n", COO_var);
//        printf("temp %.2f:\r\n", TMP_var);
//        printf("press %.2f:\r\n\n\n", APM_var);
        
        
        // 4. Depending on the current state, act accordingly.
        switch(NST_var){
            // INIT state
            case 0:
                // If the proper condition is met: proceed to the next state.
                if (HAG_var > LIFT_THRESHOLD){
                    NST_var = 1;
                    set_global_state(NST_var);
                }
                break;
                
            // LIFT state
            case 1:
                // If the proper condition is met: proceed to the next state.
                if (HAG_var > DRP1_THRESHOLD){
                    NST_var = 2;
                    set_global_state(NST_var);
                }
                break;
                
            // DRP1 state
            case 2:
                // If the proper condition is met: proceed to the next state.
                if (HAG_var < BWA_BLADES_THRESHOLD){                  
                    activate_brnwr_blades(true); //--------------------------------------------------------------Most important line of code!-------------
                    NST_var = 3;
                    set_global_state(NST_var);
                }
                break;
                
            // BWA state
            case 3:
                // If the proper condition is met: proceed to the next state.
                // In this case check if BWA is properly activated.!
                if (HAG_var < BWA_LEGS_THRESHOLD){
                    activate_brnwr_legs(true);
                    NST_var = 4;
                    set_global_state(NST_var);
                }
                break;
                
            // DRP2 state
            case 4:
                // If the proper condition is met: proceed to the next state.
                if (HAG_var < TDW_THRESHOLD){
                    NST_var = 5;
                    set_global_state(NST_var);
                }
                break;
                
            // TDW state
            case 5:
                NOP();
                break;
            
            default:
                NOP();
        }
        
        
        // 5. Construct the 32 byte telemetry message
        telemetry_package.COO_H = (COO_var >> 8) & 0xFF;;
        telemetry_package.COO_L = COO_var & 0xFF;
        
        //Convert APM_var to APM_tel
        APM_tel = (uint16_t)((APM_var - 95000.0) * 6);
        
        telemetry_package.APM_H = (APM_tel >> 8) & 0xFF;;
        telemetry_package.APM_L = APM_tel & 0xFF;
        
        telemetry_package.ACX_H = (ACX_var >> 8) & 0xFF;;
        telemetry_package.ACX_L = ACX_var & 0xFF;
        
        telemetry_package.ACY_H = (ACY_var >> 8) & 0xFF;;
        telemetry_package.ACY_L = ACY_var & 0xFF;
        
        telemetry_package.ACZ_H = (ACZ_var >> 8) & 0xFF;;
        telemetry_package.ACZ_L = ACZ_var & 0xFF;
        
        telemetry_package.GYX_H = (GYX_var >> 8) & 0xFF;;
        telemetry_package.GYX_L = GYX_var & 0xFF;
        
        telemetry_package.GYY_H = (GYY_var >> 8) & 0xFF;;
        telemetry_package.GYY_L = GYY_var & 0xFF;
        
        telemetry_package.GYZ_H = (GYZ_var >> 8) & 0xFF;;
        telemetry_package.GYZ_L = GYZ_var & 0xFF;
        
        //Convert TMP_var to TMP_tel
        TMP_tel = (uint8_t)(TMP_var * 5);
        
        telemetry_package.TMP = (uint8_t)(TMP_tel);

        telemetry_package.BVO = (uint8_t)(BVO_var  / 257);

        telemetry_package.CVO = (uint8_t)(CVO_var / 257);
        
        telemetry_package.SPD_H = (SPD_var >> 8) & 0xFF;;
        telemetry_package.SPD_L = SPD_var & 0xFF;

        UPT_tel = UPT_var / 10;
        telemetry_package.UPT_H = (UPT_tel >> 8) & 0xFF;;
        telemetry_package.UPT_L = UPT_tel & 0xFF;
        
        if(HAG_var < 0){
            HAG_tel = 0;
        }
        else{
            HAG_tel = (uint16_t)(HAG_var * 120);
        }
        
        telemetry_package.HAG_H = (HAG_tel >> 8) & 0xFF;;
        telemetry_package.HAG_L = HAG_tel & 0xFF;
        
        APR_tel = (uint16_t)((APR_var - 95000.0) * 6);
        
        telemetry_package.APR_H = (APR_tel >> 8) & 0xFF;;
        telemetry_package.APR_L = APR_tel & 0xFF;
        
        telemetry_package.RTL = RTL_var;
        
        telemetry_package.RBR = RBR_var;
        
        telemetry_package.RAC = RAC_var;
        
        telemetry_package.RGS = RGS_var;
        
        telemetry_package.NST = NST_var;
        
        // 6. Send the 32 byte telemetry message.
        sendW_TX_PAYLOAD_NOACK((uint8_t*)&telemetry_package, 32);
    }
}