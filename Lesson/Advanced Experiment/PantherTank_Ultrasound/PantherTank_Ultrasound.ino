/***********************************************************************
         __                                                          _
        / /        _____  __    __  _          _   (_)   ________   | |
       / /____   / _____) \ \  / / | |   __   | |  | |  (  ______)  | |_____
      / / ___/  | |_____   \ \/ /  | |  /  \  | |  | |  | |______   |  ___  |
     / /\ \     | |_____|   \  /   | | / /\ \ | |  | |  (_______ )  | |   | |
    / /  \ \__  | |_____    / /    | |/ /  \ \| |  | |   ______| |  | |   | |
   /_/    \___\  \______)  /_/      \__/    \__/   |_|  (________)  |_|   |_|


   Keywish Tech firmware

   Copyright (C) 2015-2020

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation, in version 3.
   learn more you can see <http://www.gnu.org/licenses/>.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.


   [Title]
   [Diagram]
*/
#include "Panther-Tank.h"
#include "ProtocolParser.h"
#include "Sounds.h"
#include "debug.h"

ProtocolParser *mProtocol = new ProtocolParser();
#if (EM_MOTOR_SHIELD_BOARD_VERSION <= 2)
Tank mTank(mProtocol, TA_AIN1_PIN, TA_AIN2_PIN, TA_PWMA_PIN, TA_BIN1_PIN, TA_BIN2_PIN, TA_PWMB_PIN, TA_STANBY_PIN);
#else
Tank mTank(mProtocol, TA_AIN1_PIN, TA_BIN1_PIN, TA_PWMA_PIN, TA_PWMB_PIN);
#endif

void setup() {
  Serial.begin(9600);
  mTank.init();
  mTank.SetBuzzerPin(TA_BUZZER_PIN);
  mTank.SetRgbPin(TA_RGB_PIN);
  mTank.Sing(S_connection);
  mTank.SetSpeed(0);
  mTank.SetUltrasonicPin(TA_TRIG_PIN, TA_ECHO_PIN, TA_SERVO_PIN);
  mTank.mUltrasonic->SetServoBaseDegree(86);
  mTank.mUltrasonic->SetServoDegree(90);
  delay(500);
}

void HandleUltrasonicAvoidance()
{
    uint16_t UlFrontDistance,UlLeftDistance,UlRightDistance;
    UlFrontDistance =  mTank.mUltrasonic->GetUltrasonicFrontDistance();
    DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance = %d \n", UlFrontDistance);
    if (UlFrontDistance < UL_LIMIT_MIN)
    {
        mTank.SetSpeed(80);
        mTank.GoBack();
        delay(200);
    }
    if(UlFrontDistance < UL_LIMIT_MID)
    {
        mTank.KeepStop();
        delay(100);
        UlRightDistance = mTank.mUltrasonic->GetUltrasonicRightDistance();
        UlLeftDistance = mTank.mUltrasonic->GetUltrasonicLeftDistance();
        if((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX)){
            mTank.SetSpeed(100);
            mTank.TurnRight();
            delay(400);
        } else if((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX)){
            mTank.SetSpeed(100);
            mTank.TurnLeft();
            delay(400);
        } else if((UlRightDistance < UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MIN) ){
            mTank.SetSpeed(400);
             mTank.TurnLeft();
            delay(800);
        }
    } else{
          mTank.SetSpeed(80);
          mTank.GoForward();
      }
}

void loop() {
  mProtocol->RecevData();
  HandleUltrasonicAvoidance();
  switch (mTank.GetStatus()) {
    case E_FORWARD:
      mTank.LightOn();
      break;
    case E_LEFT:
      mTank.LightOn(E_RGB_LEFT);
      break;
    case E_RIGHT:
      mTank.LightOn(E_RGB_RIGHT);
      //   mTank.Sing(S_OhOoh);
      break;
    case E_BACK:
      mTank.LightOn(E_RGB_ALL, RGB_RED);
      break;
    case E_STOP:
      mTank.LightOff();
      break;
    case E_SPEED_UP:
      mTank.mBuzzer->_tone(5000, 50, 20);
      mTank.LightOn(E_RGB_ALL, mTank.GetSpeed() * 2.5);
      break;
    case E_SPEED_DOWN:
      mTank.mBuzzer->_tone(1000, 50, 20);
      mTank.LightOn(E_RGB_ALL, mTank.GetSpeed() * 2.5);
      break;
    default:
      break;
  }
}
