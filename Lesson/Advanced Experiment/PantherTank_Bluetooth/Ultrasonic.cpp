#include "Ultrasonic.h"

static uint16_t history = 0;
Ultrasonic::Ultrasonic(byte trig_pin, byte echo_pin, byte servo_pin)
{
    TrigPin = trig_pin;
    EchoPin = echo_pin;
    ServoPin = servo_pin;
    pinMode(EchoPin, INPUT);
    pinMode(TrigPin, OUTPUT);
    pinMode(ServoPin, OUTPUT);
    mForwardServo.attach(ServoPin);
}

Ultrasonic::init()
{
    pinMode(EchoPin, INPUT);
    pinMode(TrigPin, OUTPUT);
    pinMode(ServoPin, OUTPUT);
    mForwardServo.attach(ServoPin);
}
uint16_t Ultrasonic::GetUltrasonicFrontDistance()
{
    do {
        digitalWrite(TrigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(TrigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(TrigPin, LOW);
        FrontDistance = pulseIn(EchoPin, HIGH) / 58.00;
    } while (FrontDistance > UL_LIMIT_MAX);
    return FrontDistance;
}

uint16_t Ultrasonic::GetUltrasonicLeftDistance()
{
    SetServoDegree(170);
    history = 0;
    LeftDistance = GetUltrasonicFrontDistance();
    SetServoDegree(90);
    return LeftDistance;
}

uint16_t Ultrasonic::GetUltrasonicRightDistance()
{
    SetServoDegree(5);
    history = 0;
    RightDistance = GetUltrasonicFrontDistance(); 
    SetServoDegree(90);
    return RightDistance;
}

Ultrasonic::SetServoBaseDegree(uint8_t base)
{
    ServoBaseDegree = base;
}

Ultrasonic::SetServoDegree(int Angle)
{
	int servo_degree;
	if (Angle > 180) {
		servo_degree = 180;
	} else if( Angle < 10) {
        servo_degree = 10;
    } else if (Angle == 90) {
		servo_degree = ServoBaseDegree;
	} else {
		servo_degree = 90 - ServoBaseDegree + Angle;   // 180-degree-diff
	}
  mForwardServo.write(servo_degree);
	delay(300);
}
