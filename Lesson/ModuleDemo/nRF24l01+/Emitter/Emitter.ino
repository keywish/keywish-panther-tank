//Transmitter program

#include <SPI.h>
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

#define EM_MOTOR_SHIELD_BOARD_VERSION 3

#if (EM_MOTOR_SHIELD_BOARD_VERSION > 2)
    #define NRF24L01_CE 10
    #define NRF24L01_CSN 8
#else
    #define NRF24L01_CE 10
    #define NRF24L01_CSN 9
#endif

Nrf24l Mirf = Nrf24l(NRF24L01_CE, NRF24L01_CSN);
byte value[8] = {0xAA, 0x01, 0x01, 0x06, 0x23, 0x00, 0x2B, 0x55};
uint8_t  Addr = 0x01;

const uint64_t pipe = 0xE8E8F0F0E1LL;
void setup()
{
    Serial.begin(9600);
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    // Set your own address (sender address) using 5 characters
    Mirf.setRADDR((byte *)"ABCDE");
    Mirf.channel = 90;              //Set the channel used
    Mirf.config();
}

void loop()
{
	Mirf.setTADDR((uint8_t *)"MotorDrive");           //Set the receiver address
    Mirf.payload = sizeof(value);
	//value = random(255);                      //0-255 random number
	Mirf.send(value);                //Send instructions, send random number value
	Serial.println("Wait for sending.....");
	while (Mirf.isSending()) delay(2);        //Until you send successfully, exit the loop
	Serial.println("Send success:");
//	Serial.println(value);
	delay(1000);
}
