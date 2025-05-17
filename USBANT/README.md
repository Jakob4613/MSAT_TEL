# Functionality
The core functionality of the USBANT is to receive the 2.4Ghz signal transmitted by the satellite, decoding this 32-byte telemetry message, transforming it into a JSON format with appropriate identifier and writing it to a 115200 baud serial stream. This decoding process has to be set up meticulously in combination with the NUTS software, to guarantee that the sent bytes align with the received bytes and that the correct bytes. No measurement transformations are applied, as these are applied in the python-backend of the computer software.

# Hardware
## Arduino Uno
The computations executed by the USBANT are performed by an Arduino UNO. It can be bought from [Tinytronics](https://www.tinytronics.nl/en/development-boards/microcontroller-boards/arduino-compatible/arduino-uno-r3)

## Arduino Uno NRF24L01+ shield
Attached to the Arduino uno, and connecting it to the 2.4 Ghz tranceiver is a shield. The gerber files for this shield can be downloaded from [Openhardware](https://www.openhardware.io/view/694/Arduino-UNO-NRF24L01-Shield) and it is completed with the appropriate headers and a single 10 uF capacitor.

## NRF24L01+ 2.4 Ghz tranceiver
On the 8-pin female headers attached to the top of the shield, an NRF24L01+ including pa + lna and SMA connector can be plugged in. It can be bought from [Tinytronics](https://www.tinytronics.nl/en/communication-and-signals/wireless/rf/modules/nrf24l01-wireless-module-with-pa-and-lna).

## 3D-printed enclosure
The STL files used for printing the enclosure of the USBANT can be found in the `Housing_STL` folder. It consists of two parts that snap into each other around the Arduino uno, its shield and the tranceiver. Printing it from either PLA or PETG should perform fine. After assembling the complete USBANT, make sure to wrap the USBANT with aluminium shielding tape. This layer of shielding tape should be properly connected to the ground of the Arduino’s USB-A connector. Make sure that no ground-loops can occur by not connecting this shielding to the SMA connector. This protects the sensitive electronics for possible incoming interference.

# Schematic / Connections
| **Arduino Uno Pin** | **NRF24L01+ Pin** |
|-------------|----------------|
| D2          | IRQ            |
| D9          | CE             |
| D10         | CSN            |
| D11         | MOSI           |
| D12         | MISO           |
| D13         | SCK            |

# Improvements
 - The arduino Uno was selected due to its ease of use. However, it was never validated whether this is the most effective microcontroller for this task.
 - One of the mounting holes of the shield does not align with the mounting holes of the arduino.
 - The SMD pads for the NRF24L01+ module, does not work for every NRF24 module. Caution should be taken when connecting this.