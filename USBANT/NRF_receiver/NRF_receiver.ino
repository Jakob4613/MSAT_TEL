#include <SPI.h>                            /* to handle the communication interface with the modem*/
#include <nRF24L01.h>                       /* to handle this particular modem driver*/
#include <RF24.h>                           /* the library which helps us to control the radio modem*/

//barebones USBANT
//RF24 radio(8,10);                           /* Creating instance 'radio'  ( CE , CSN )   CE -> D7 | CSN -> D8 */                               
//Version 2 USBANT aluwrap
RF24 radio(9,10);                           /* Creating instance 'radio'  ( CE , CSN )   CE -> D7 | CSN -> D8 */                               
const byte Address[6] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};            /* Address from which data to be received */

struct TelemetryPackage {
  unsigned int COO:16;
  unsigned int APM:16;
  int ACX:16;
  int ACY:16;
  int ACZ:16;
  int GYX:16;
  int GYY:16;
  int GYZ:16;
  unsigned int TMP:8;
  unsigned int BVO:8;
  unsigned int CVO:8;
  unsigned int SPD:16;
  unsigned int UPT:16;
  unsigned int HAG:16;
  unsigned int APR:16;
  unsigned int RTL:1;
  unsigned int RBR:1;
  unsigned int RAC:1;
  unsigned int RGS:1;
  unsigned int NST:4;
  unsigned int NULL_1:8;
  unsigned int NULL_2:8;
  unsigned int NULL_3:8;
  unsigned int NULL_4:8;
};

TelemetryPackage telemetryData;  // Declare the struct variable


byte tel_packet[32];  // 32-byte array for storing the serialized struct


static int serial_putc(char c, FILE *stream) {
    Serial.write(c);
    return 0;
}


void printTelemetry(const TelemetryPackage& telemetryData) {
  Serial.print("{\"COO\":");
  Serial.print(telemetryData.COO);
  Serial.print(",\"APM\":");
  Serial.print(telemetryData.APM);
  Serial.print(",\"ACX\":");
  Serial.print(telemetryData.ACX);
  Serial.print(",\"ACY\":");
  Serial.print(telemetryData.ACY);
  Serial.print(",\"ACZ\":");
  Serial.print(telemetryData.ACZ);
  Serial.print(",\"GYX\":");
  Serial.print(telemetryData.GYX);
  Serial.print(",\"GYY\":");
  Serial.print(telemetryData.GYY);
  Serial.print(",\"GYZ\":");
  Serial.print(telemetryData.GYZ);
  Serial.print(",\"TMP\":");
  Serial.print(telemetryData.TMP);
  Serial.print(",\"BVO\":");
  Serial.print(telemetryData.BVO);
  Serial.print(",\"CVO\":");
  Serial.print(telemetryData.CVO);
  Serial.print(",\"RTL\":");
  Serial.print(telemetryData.RTL);
  Serial.print(",\"RBR\":");
  Serial.print(telemetryData.RBR);
  Serial.print(",\"RAC\":");
  Serial.print(telemetryData.RAC);
  Serial.print(",\"RGS\":");
  Serial.print(telemetryData.RGS);
  Serial.print(",\"NST\":");
  Serial.print(telemetryData.NST);
  Serial.print(",\"SPD\":");
  Serial.print(telemetryData.SPD);
  Serial.print(",\"UPT\":");
  Serial.print(telemetryData.UPT);
  Serial.print(",\"HAG\":");
  Serial.print(telemetryData.HAG);
  Serial.print(",\"APR\":");
  Serial.print(telemetryData.APR);
  Serial.print(",\"NL1\":");
  Serial.print(telemetryData.NULL_1);
  Serial.print(",\"NL2\":");
  Serial.print(telemetryData.NULL_2);
  Serial.print(",\"NL3\":");
  Serial.print(telemetryData.NULL_3);
  Serial.print(",\"NL4\":");
  Serial.print(telemetryData.NULL_4);
  Serial.println("}");
}

FILE serial_stdout;

void setup() {
  Serial.begin(115200);
  if(radio.begin()){
    //Serial.println("NRF24 Correctly initialized.");
  }
  else{
    Serial.println("ERROR: NRF24 not initialized");
  }
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_8);
  //radio.disableAckPayload();
  //radio.disableCRC();
  radio.setPayloadSize(32);
  radio.setAutoAck(false);
  radio.setAddressWidth(3);
  radio.setChannel(0x28);
  radio.openReadingPipe(1, Address);        /* Sets the address of receiver from which program will receive the data*/
  radio.startListening();			              /*Setting modem in Receiver mode*/

  
    fdev_setup_stream(&serial_stdout, serial_putc, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial_stdout;  // Redirect stdout to Serial
  
  
  radio.printPrettyDetails();
  //radio.printDetails();


    
}

void loop() {


  if (radio.available()){
        

    radio.read(&tel_packet, sizeof(tel_packet));
    memcpy(&telemetryData, tel_packet, sizeof(telemetryData));
    
    printTelemetry(telemetryData);


    // Code for printing the raw telemetry packages!
//    for (size_t i = 0; i < sizeof(tel_packet); i++) {
//        if ((uint8_t)tel_packet[i] < 0x10) {
//            Serial.print("0"); // Add leading zero for single-digit hex values
//        }
//        Serial.print((uint8_t)tel_packet[i], HEX);
//        Serial.print(" "); // Space for readability
//    }
//    Serial.println("\n");
  }
}