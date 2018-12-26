//Receiver program
#include <SPI.h>
#include "nRF24L01.h"

#define EM_MOTOR_SHIELD_BOARD_VERSION 3

#if (EM_MOTOR_SHIELD_BOARD_VERSION > 2)
    #define NRF24L01_CE 10
    #define NRF24L01_CSN 8
#else
    #define NRF24L01_CE 10
    #define NRF24L01_CSN 9
#endif

Nrf24l Mirf = Nrf24l(NRF24L01_CE, NRF24L01_CSN);
String SmartCarName = "MotorDrive";
byte value[12];

void setup()
{
    Serial.begin(9600);
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();

    Mirf.setRADDR((uint8_t *)SmartCarName.c_str()); //Set your own address (receiver address) 
    Mirf.payload = sizeof(value);
    Mirf.channel = 90;             //Set the used channel
    Mirf.config();
    Serial.println("Listening...");  //Start listening to received data
}

void loop()
{
  if (Mirf.dataReady()) { //When the program is received, the received data is output from the serial port
    Mirf.getData(value);
    Serial.print("Got data: ");
      for (int i = 0; i < 12; i++) {
        Serial.print(value[i], HEX);Serial.print(" ");
       }
     Serial.println();
  }
}
