# Functionality
The core functionality of the NUTS flight computer is measuring the required sensor values (CO2 contents, acceleration, temperature etc.), respectively deploying the autogyro and impact legs at an altitude of 200 and 50m during descent, and transmitting the acquired sensor values to the groundstation.

# Hardware
## PIC18F16Q20 Microcontroller
[datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/PIC18F06-16Q20-Microcontroller-Data-Sheet-DS40002387.pdf)

## ICP-10111 Barometer
[datasheet](https://invensense.tdk.com/wp-content/uploads/2021/06/DS-000177-ICP-10111-v1.3.pdf)

## ICM-20948 Inertial Measurement Unit
[datasheet](https://invensense.tdk.com/wp-content/uploads/2016/06/DS-000189-ICM-20948-v1.3.pdf)

## ENS160 Gas sensor
[datasheet](https://www.mouser.com/datasheet/2/1081/SC_001224_DS_1_ENS160_Datasheet_Rev_0_95-2258311.pdf)

## NRF24L01+ 2.4Ghz tranceiver
[datasheet](https://docs-be.nordicsemi.com/bundle/nRF24L01P_PS_v1.0/raw/resource/enus/nRF24L01P_PS_v1.0.pdf)

# Schematic / Connections
The schematic and table below indicates all of the connections from the PIC18F16Q20 to its peripherals. 

| **PIC Pin** | **Connection**                           | **Description**                                                                 |
|-------------|-------------------------------------------|---------------------------------------------------------------------------------|
| Pin 1       | MCLR                                      | Used for In-Circuit Serial Programming (ICSP)                                   |
| Pin 2       | CSN                                       | Chip Select for SPI (connected to NRF24L01+)                                    |
| Pin 3       | SDO                                       | Slave Data Out for SPI (connected to NRF24L01+)                                 |
| Pin 4       | V_burn                                    | Voltage sensing across burnwire (with series current limiting resistor)         |
| Pin 5       | BUTTON                                    | Connected to configuration button (with parallel pull-up resistor)              |
| Pin 6       | BUZZER                                    | Connected to buzzer; writing high activates it                                  |
| Pin 7       | BATT_measure                              | Connected to 9V battery via voltage divider for voltage sensing                 |
| Pin 8       | SCK                                       | SPI Clock (connected to NRF24L01+)                                              |
| Pin 9       | SDI                                       | Slave Data In for SPI (connected to NRF24L01+)                                  |
| Pin 10      | VDDIO3                                    | MCU Power, connected to 3.3V                                                    |
| Pin 11      | VDDIO2                                    | MCU Power, connected to 1.8V for I2C bus compatibility                          |
| Pin 12      | SCL                                       | I2C Clock (connected to I2C bus)                                                |
| Pin 13      | SDA                                       | I2C Data (connected to I2C bus)                                                 |
| Pin 14      | E_burn                                    | Connected to burnwire chip; writing high activates burnwire                     |
| Pin 15      | ICSPCLK                                   | Used for In-Circuit Serial Programming (ICSP)                                   |
| Pin 16      | ICSPDAT                                   | Used for In-Circuit Serial Programming (ICSP)                                   |
| Pin 17      | GND                                       | Ground                                                                           |
| Pin 18      | VDD                                       | MCU Power, connected to 3.3V                                                    |
| Pin 19      | CLKIN                                     | External oscillator input                                                       |
| Pin 20      | CLKOUT                                    | External oscillator output                                                      |

![MCU schematic](./schematics/MCU.png)

# Improvements
 - As of yet, not a lot of research has been put into clock speeds and the effect on the microcontroller. The main benefit of increasing the clock speed is the increase in telemetry sending rate. We have currently chosen a conservative clock speed to ensure MCU stability. An MCU crash at burnwire deployment could majorly influence the functioning of our satellite.
