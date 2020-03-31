#include<Arduino.h>
#include<Wire.h>
#include "Emakefun_MotorDriver.h"

Emakefun_MotorDriver mMotorDriver = Emakefun_MotorDriver(0x60,MOTOR_DRIVER_BOARD_V5);
Emakefun_DCMotor *DCMotor_1 = mMotorDriver.getMotor(M1);
Emakefun_DCMotor *DCMotor_2 = mMotorDriver.getMotor(M2);

void setup()
{
  Serial.begin(9600);
  mMotorDriver.begin(50);
}

void loop()
{
  DCMotor_1->setSpeed(255);
  DCMotor_2->setSpeed(255);
  DCMotor_1->run(FORWARD);
  DCMotor_2->run(FORWARD);
  delay(1000);
  DCMotor_1->run(BACKWARD);
  DCMotor_2->run(BACKWARD);
  delay(1000);;
  DCMotor_1->run(FORWARD);
  DCMotor_2->run(BACKWARD);
  delay(1000);
  DCMotor_1->run(BACKWARD);
  DCMotor_2->run(FORWARD);
  delay(1000);
  DCMotor_1->run(BRAKE);
  DCMotor_2->run(BRAKE);
  delay(1000);
}
