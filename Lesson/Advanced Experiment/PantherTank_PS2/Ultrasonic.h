
#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

/* Includes ------------------------------------------------------------------*/
#include <Arduino.h>
#include "Servo.h"

#define UL_LIMIT_MIN 16
#define UL_LIMIT_MID 20
#define UL_LIMIT_MAX 500

class Ultrasonic
{
  private:
    byte EchoPin, TrigPin, ServoPin;
    uint8_t ServoBaseDegree;

  public:
    Servo mForwardServo;
    byte determine;
    uint16_t FrontDistance;
    uint16_t RightDistance;
    uint16_t LeftDistance;
    Ultrasonic(byte trig_pin, byte echo_pin, byte servo_pin);
   void init(void);
    uint16_t GetUltrasonicFrontDistance(void);
    uint16_t GetUltrasonicLeftDistance(void);
    uint16_t GetUltrasonicRightDistance(void);
    void SetServoBaseDegree(uint8_t base);
    void SetServoDegree(int degree);
};
#endif
