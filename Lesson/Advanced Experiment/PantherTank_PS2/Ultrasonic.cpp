#include "Ultrasonic.h"

static uint16_t history = 0;
#if ARDUINO > 10609
Ultrasonic::Ultrasonic(byte trig_pin=TA_TRIG_PIN, byte echo_pin== TA_ECHO_PIN, byte servo_pin=TA_SERVO_PIN)
#else 
 Ultrasonic::Ultrasonic(byte trig_pin, byte echo_pin, byte servo_pin)
#endif
{
    TrigPin = trig_pin;
    EchoPin = echo_pin;
    ServoPin = servo_pin;
    pinMode(EchoPin, INPUT);
    pinMode(TrigPin, OUTPUT);
    pinMode(ServoPin, OUTPUT);
    mForwardServo.attach(ServoPin);
}

 void Ultrasonic::init(void)
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

void Ultrasonic::SetServoBaseDegree(uint8_t base)
{
    ServoBaseDegree = base;
}

void Ultrasonic::SetServoDegree(int Angle)
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
