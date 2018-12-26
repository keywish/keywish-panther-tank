#ifndef _PANTHER-TANK_H_
#define _PANTHER-TANK_H_

#define VERSION "3.0"

#include <stdint.h>
#include "SPI.h"
#include "Arduino.h"
#include "SmartCar.h"
#include "IRremote.h"
#include "PS2X_lib.h"  //for v1.6
#include "Buzzer.h"
#include "RGBLed.h"
#include "ProtocolParser.h"
#include "Ultrasonic.h"
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

#define EM_MOTOR_SHIELD_BOARD_VERSION 3
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    #define TA_IR_PIN 8
    #define TA_AIN2_PIN 11
    #define TA_BIN2_PIN 2
    #define TA_STANBY_PIN 7
    #define TA_SERVO_PIN 13
    #define TA_ECHO_PIN A0
    #define TA_TRIG_PIN A1
    #define TA_BUZZER_PIN 9
    #define TA_PS2X_CLK A4
    #define TA_PS2X_CMD A1
    #define TA_PS2X_CS  A2
    #define TA_PS2X_DAT A0
#else
    #define TA_BATTERY_PIN A0
    #define TA_IR_PIN 12
    #define TA_SERVO_PIN 7
    #define TA_ECHO_PIN A2
    #define TA_TRIG_PIN A1
    #define TA_BUZZER_PIN 9
    #define TA_PS2X_CLK 13
    #define TA_PS2X_CMD 11
    #define TA_PS2X_CS  10
    #define TA_PS2X_DAT 12  
    #define TA_CSN_PIN 8
    #define TA_CE_PIN 10
#endif

#define TA_AIN1_PIN 3
#define TA_PWMA_PIN 5
#define TA_BIN1_PIN 4
#define TA_PWMB_PIN 6
#define TA_RGB_PIN A3

typedef enum
{
    E_RGB_ALL = 0,
    E_RGB_RIGHT = 1,
    E_RGB_LEFT = 2
} E_RGB_INDEX;

class Tank: public SmartCar {

private :
    uint8_t Ain1Pin, PwmaPin, Bin1Pin, PwmbPin;
 #if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    uint8_t Ain2Pin, Bin2Pin, StandbyPin;
 #endif
    uint8_t BatteryPin;
    uint8_t IrPin;      // Infrared remoter pin
    uint8_t BuzzerPin;  // Buzzer pin
    uint8_t RgbPin;     // Rgb pin
    uint8_t Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin;    // for Ps2 remoter
    uint8_t ServoBaseDegree; //correct degree for stop
    uint8_t EchoPin,TrigPin,ServoPin;
    uint8_t CePin,CsnPin; //Nrf24L01 pin
    int  NrfData;
    ST_PROTOCOL SendData;
    ProtocolParser *mProtocolPackage;

public :
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    Tank(ProtocolParser *Package, uint8_t ain1, uint8_t ain2, uint8_t pwma, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby);
#else
    Tank(ProtocolParser *Package, uint8_t ain1, uint8_t bin1, uint8_t pwma, uint8_t pwmb);
#endif
    ~Tank();
    IRremote  *mIrRecv;

    PS2X *mPs2x;
    Buzzer *mBuzzer;
    RGBLed *mRgb;
    Ultrasonic *mUltrasonic;
    Nrf24l *Mirf;
    void GoForward(void);
    void GoBack(void);
    void TurnLeft(void);
    void TurnRight(void);
    void KeepStop(void);
    void Drive(void);
    void Drive(int degree);
    // void SetSpeed(uint8_t s);
    void SetIrPin(uint8_t pin = TA_IR_PIN);
    void SetRgbPin(uint8_t pin = TA_RGB_PIN);
    void LightOn(E_RGB_INDEX index = E_RGB_ALL, long Color = RGB_WHITE);
    void LightOff(E_RGB_INDEX index = E_RGB_ALL);
    void SetBuzzerPin(uint8_t pin = TA_BUZZER_PIN);
    void Sing(byte songName);
    void SendUltrasonicData();
    void PianoSing(ST_MUSIC_TYPE music);
    int SetPs2xPin(uint8_t clk = TA_PS2X_CLK, uint8_t cmd = TA_PS2X_CMD, uint8_t att = TA_PS2X_CS, uint8_t dat = TA_PS2X_DAT);
    int ResetPs2xPin(void);
    void SetBatteryCheckPin(uint8_t pin);
    uint16_t GetBatteryValue(void);
    void SendBatteryPackage(uint16_t battery_value);
    void SendVersionPackage(void);
    void init(void);
    void SetUltrasonicPin(uint8_t Trig_Pin = TA_TRIG_PIN, uint8_t Echo_Pin = TA_ECHO_PIN, uint8_t Sevo_Pin = TA_SERVO_PIN);
    #if (EM_MOTOR_SHIELD_BOARD_VERSION > 2)
    void SetNRF24L01Pin(uint8_t CE_Pin = TA_CE_PIN, uint8_t CSN_Pin = TA_CSN_PIN);
    void InitNrf24L01(char *Raddr);
    int  GetNrf24L01(void);
    #endif
};

#endif  /* _AURORARACING_H_ */
