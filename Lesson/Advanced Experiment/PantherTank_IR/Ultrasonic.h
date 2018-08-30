
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
    Ultrasonic::init();
    uint16_t GetUltrasonicFrontDistance();
    uint16_t GetUltrasonicLeftDistance();
    uint16_t GetUltrasonicRightDistance();
    SetServoBaseDegree(uint8_t base);
    SetServoDegree(int degree);
};
#endif
