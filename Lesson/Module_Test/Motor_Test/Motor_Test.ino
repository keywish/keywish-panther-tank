#include<Arduino.h>
#include<Wire.h>
#include "Emakefun_MotorDriver.h"

Emakefun_MotorDriver mMotorDriver = Emakefun_MotorDriver();
Emakefun_DCMotor *DCMotor_1 = mMotorDriver.getMotor(1);
Emakefun_DCMotor *DCMotor_2 = mMotorDriver.getMotor(2);

void setup()
{
  Serial.begin(9600);
  mMotorDriver.begin(50);
  DCMotor_1->setSpeed(100);
  DCMotor_2->setSpeed(100);
}

void loop()
{
  DCMotor_1->run(FORWARD);
  DCMotor_2->run(FORWARD);
  delay(1000);
  DCMotor_1->run(BACKWARD);
  DCMotor_2->run(BACKWARD);
  delay(1000);
  DCMotor_1->run(FORWARD);
  DCMotor_2->run(BACKWARD);
  delay(1000);
  DCMotor_1->run(BACKWARD);
  DCMotor_2->run(FORWARD);
  delay(1000);
  DCMotor_1->run(RELEASE);
  DCMotor_2->run(RELEASE);
  delay(1000);
}
