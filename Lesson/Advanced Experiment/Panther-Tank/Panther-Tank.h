#ifndef _PANTHER-TANK_H_
#define _PANTHER-TANK_H_

#define VERSION "PT-201807016"

#include <stdint.h>
#include "Arduino.h"
#include "SmartCar.h"
#include "IRremote.h"
#include "PS2X_lib.h"  //for v1.6
#include "Buzzer.h"
#include "RGBLed.h"
#include "InfraredTracing.h"
#include "ProtocolParser.h"
#include "Ultrasonic.h"
#define TA_IR_PIN 8
#define TA_AIN1_PIN 3
#define TA_AIN2_PIN 11
#define TA_PWMA_PIN 5
#define TA_BIN1_PIN 4
#define TA_BIN2_PIN 2
#define TA_PWMB_PIN 6
#define TA_STANBY_PIN 7
#define TA_SERVO_PIN 13
#define TA_ECHO_PIN A0
#define TA_TRIG_PIN A1
#define TA_RGB_PIN A3
#define TA_BUZZER_PIN 9
#define TA_INFRARED_TRACING_PIN1 A0
#define TA_INFRARED_TRACING_PIN2 A1
#define TA_INFRARED_TRACING_PIN3 A2
#define TA_INFRARED_TRACING_PIN4 A4
#define TA_INFRARED_TRACING_PIN5 A5
#define TA_PS2X_CLK A4
#define TA_PS2X_CMD A1
#define TA_PS2X_CS  A2
#define TA_PS2X_DAT A0

typedef enum
{
    E_RGB_ALL = 0,
    E_RGB_RIGHT = 1,
    E_RGB_LEFT = 2
} E_RGB_INDEX;

class Tank: public SmartCar {

private :
    uint8_t Ain1Pin, Ain2Pin, PwmaPin, Bin1Pin, Bin2Pin, PwmbPin, StandbyPin;
    uint8_t IrPin;      // Infrared remoter pin
    uint8_t BuzzerPin;  // Buzzer pin
    uint8_t RgbPin;     // Rgb pin
    uint8_t InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3, InfraredTracingPin4, InfraredTracingPin5;    // for Infrared tracing pin
    uint8_t Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin;    // for Ps2 remoter
    uint8_t ServoBaseDegree; //correct degree for stop
    uint8_t EchoPin,TrigPin,ServoPin;

    ProtocolParser *mProtocolPackage;

public :
  //  Tank(ProtocolParser *Package, uint8_t Servo_Pin = TA_SERVO_PIN, uint8_t bin1 = TA_BIN1_PIN, uint8_t bin2 = TA_BIN2_PIN, uint8_t pwmb = TA_PWMB_PIN, uint8_t standby = TA_STANBY_PIN);
  //  Tank(ProtocolParser *Package, uint8_t ain1 = TA_AIN1_PIN, uint8_t ain2 = TA_AIN2_PIN, uint8_t pwma = TA_PWMA_PIN, uint8_t bin1 = TA_BIN1_PIN, uint8_t bin2 = TA_BIN2_PIN, uint8_t pwmb = TA_PWMB_PIN, uint8_t standby = TA_STANBY_PIN);
    Tank(ProtocolParser *Package, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby);
    Tank(ProtocolParser *Package, uint8_t ain1, uint8_t ain2, uint8_t pwma, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby);
    ~Tank();
    IRremote  *mIrRecv;

    PS2X *mPs2x;
    Buzzer *mBuzzer;
    RGBLed *mRgb;
    InfraredTracing *mInfraredTracing;
    Ultrasonic *mUltrasonic;
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
    float PianoSing(byte b[]);
    void SetInfraredTracingPin(uint8_t Pin1 = TA_INFRARED_TRACING_PIN1, uint8_t Pin2 = TA_INFRARED_TRACING_PIN2, uint8_t Pin3 = TA_INFRARED_TRACING_PIN3);
    int SetPs2xPin(uint8_t clk = TA_PS2X_CLK, uint8_t cmd = TA_PS2X_CMD, uint8_t att = TA_PS2X_CS, uint8_t dat = TA_PS2X_DAT);
    int ResetPs2xPin(void);
    void SendBatteryPackage(byte *battery_value);
    void init(void);
    void SetUltrasonicPin(uint8_t Trig_Pin = TA_TRIG_PIN, uint8_t Echo_Pin = TA_ECHO_PIN, uint8_t Sevo_Pin = TA_SERVO_PIN);
};

#endif  /* _AURORARACING_H_ */
