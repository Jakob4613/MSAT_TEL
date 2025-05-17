# Mulerius Satellites Software 2025
This is the github repository associated with team ‘25. The repository is separated in the three different pieces of hardware required for the mission. These are: NUTS v2.0 (Cansat’s flight computer), the USBANT (the antenna to usb interface) and the computer (telemetry databasing and dashboarding). README’s and code are provided in the associated subfolder.

## Interfaces
The following interfaces are present in the complete system. These interfaces are highlighted here, as they XXoverstijgenXX the readme’s from the subsoftwares.
<add a diagram of the interfaces between the different code-bases>

### Computer <-> USBANT
The main interface between computer and USBANT is the serial connection. In order to assure proper functionality, make sure that the baud rates, bit parity and other serial settings align. They can be found and set up in XXX (Computer) and XXX (USBANT). In the current setup, the baud rates is set to 119200 baud and the other serial settings are left at Arduino’s default.

### USBANT <-> NUTS v2.0
The interface between USBANT and the NUTS happens between the two NRF24L01+ 2.4Ghz tranceivers. The NUTS tranceiver is set up as a transmitter, whereas the USBANT is set up as a receiver. This requires that both devices are set up in accordance with each other. The current setup is as follows:
 - The packet data size is set to 32 bytes.
 - CRC is enabled and set to a single byte.
 - Retransmission in disabled.
 - Auto acknowledgement is disabled.
 - Address size is set to 3 bits.
 - Channel is set to XXX.
 - XXXXXXXXXXXX is set to XXXX.

Setting up this configuration requires a lot of attention as the smallest misconfiguration may destroy the complete system.
