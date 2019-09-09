#ifndef _PANTHER_TANK_H_
#define _PANTHER_TANK_H_
#include <stdint.h>
#include "SPI.h"
#include "Arduino.h"
#include "SmartCar.h"
#include "Emakefun_MotorDriver.h"
#include "ProtocolParser.h"

#define FRONT 0
#define LEFT 1
#define RIGHT 2
#define MIDDLE 3
typedef enum
{
  E_EFFECT_BREATHING = 0,
  E_EFFECT_ROTATE = 1,
  E_EFFECT_FLASH = 2
} E_RGB_EFFECT;



class Tank: public SmartCar {

  private :
    uint8_t Ain1Pin, PwmaPin, Bin1Pin, PwmbPin;
    byte ServoPin;
    uint8_t BatteryPin;
    uint8_t ServoBaseDegree;
    ST_PROTOCOL SendData;
    ProtocolParser *mProtocolPackage;

  public :
    Emakefun_MotorDriver *mMotorDriver;
    Emakefun_DCMotor *DCMotor_1;
    Emakefun_DCMotor *DCMotor_2;
    Emakefun_Sensor *IR;
    Emakefun_Sensor *Buzzer;
    Emakefun_Sensor *Rgb;
    Emakefun_Sensor *UT;
    Emakefun_Sensor  *Ps2x;
    Emakefun_Sensor *Nrf;
    Emakefun_Servo *mServo1, *mServo2, *mServo3, *mServo4, *mServo5, *mServo6;
    Tank(ProtocolParser *Package, uint8_t ain1Pin, uint8_t bin1Pin);
    ~Tank(void);
    void init(void);
    void GoForward(void);
    void GoBack(void);
    void TurnLeft(void);
    void TurnRight(void);
    void KeepStop(void);
    void Drive(void);
    void Drive(int degree);
    void InitIrPin(void);
    int GetIrKey(void);
    void InitBuzzerPin(void);
    void sing(byte songName);
    void PianoSing(ST_MUSIC_TYPE music);
    void InitRgbPin(void);
    void SetRgbColor(E_RGB_INDEX index , long Color);
    void LightOff(void);
    void SetRgbEffect(E_RGB_INDEX index, long Color, uint8_t effect);
    void InitUltrasonicPin(void);
    byte GetUltrasonicValue(byte);
    void InitServoPin(void);
    void SetServoBaseDegree(uint8_t base);
    void SetServoDegree(byte pin, byte Angle);
    void InitPs2xPin(void);
    int GetPs2xKeyValue(void);
    uint16_t GetPs2xRockerAngle(byte);
    void SetBatteryCheckPin(uint8_t pin);
    uint16_t GetBatteryValue(void);
    void SendBatteryPackage(uint16_t battery_value);
    void SendUltrasonicData(void);
};

#endif  /* _AURORARACING_H_ */
