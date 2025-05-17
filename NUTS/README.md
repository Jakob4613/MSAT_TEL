# Functionality
The core functionality of the NUTS flight computer is measuring the required sensor values (CO2 contents, acceleration, temperature etc.), deploying the autogyro at an altitude of 200m during descent, and transmitting the acquired sensor values to the groundstation.

# Hardware
## PIC18F16Q20 Microcontroller

## ICP-10111 Barometer

## ICM-20948 Inertial Measurement Unit

## ENS160 Gas sensor

## NRF24L01+ 2.4Ghz tranceiver

# Schematics

# Improvements
 - As of yet, not a lot of research has been put into clock speeds and the effect on the microcontroller. The main benefit of increasing the clock speed is the increase in telemetry sending rate. We have currently chosen a conservative clock speed to ensure MCU stability. An MCU crash at burnwire deployment could majorly influence the functioning of our satellite.