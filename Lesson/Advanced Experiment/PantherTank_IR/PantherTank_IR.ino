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
#include "KeyMap.h"
#include "Sounds.h"
#include "debug.h"
#define AIN1_PIN 3
#define AIN2_PIN 11
#define PWMA_PIN 5
#define BIN1_PIN 4
#define BIN2_PIN 2
#define PWMB_PIN 6
#define STANBY_PIN 7
#define BUZZER_PIN 9
#define RGB_PIN A3
#define IR_PIN 8

ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol, TA_AIN1_PIN, TA_AIN2_PIN, TA_PWMA_PIN, BIN1_PIN, BIN2_PIN, PWMB_PIN, STANBY_PIN);

void setup() {
  Serial.begin(9600);
  mTank.init();
  mTank.SetControlMode(E_INFRARED_REMOTE_CONTROL);
  mTank.SetBuzzerPin(BUZZER_PIN);
  mTank.SetRgbPin(RGB_PIN);
  mTank.Sing(S_connection);
  mTank.SetSpeed(0);
  mTank.SetIrPin(IR_PIN);
}

void HandleInfaredRemote(byte irKeyCode)
{
  switch ((E_IR_KEYCODE)mTank.mIrRecv->getIrKey(irKeyCode)) {
    case IR_KEYCODE_STAR:
      mTank.SpeedUp(10);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "Speed+ = %d \n", mTank.Speed);
      break;
    case IR_KEYCODE_POUND:
      DEBUG_LOG(DEBUG_LEVEL_INFO, "Speed- = %d \n", mTank.Speed);
      mTank.SpeedDown(10);
      break;
    case IR_KEYCODE_UP:
      mTank.GoForward();
      break;
    case IR_KEYCODE_DOWN:
      mTank.GoBack();
      break;
    case IR_KEYCODE_OK:
      mTank.KeepStop();
      break;
    case IR_KEYCODE_LEFT:
      mTank.Drive(180);
      break;
    case IR_KEYCODE_RIGHT:
      mTank.Drive(0);
      break;
    default:
      break;
  }
  //  mTank.mIrRecv->resume();
  }

void loop() {
  mProtocol->RecevData();
  if (mTank.GetControlMode() !=  E_BLUETOOTH_CONTROL &&  mTank.GetControlMode() != E_PIANO_MODE) {
    if (mProtocol->ParserPackage()) {
        if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
          mTank.SetControlMode(mProtocol->GetControlMode());
       }
    }
  }
  switch (mTank.GetControlMode()) {
    case E_INFRARED_REMOTE_CONTROL:
      byte irKeyCode;
      if (irKeyCode = mTank.mIrRecv->getCode()) {
        //DEBUG_LOG(DEBUG_LEVEL_INFO, "irKeyCode = %lx \n", irKeyCode);
        HandleInfaredRemote(irKeyCode);
        delay(110);
      } else {
        if (mTank.GetStatus() != E_STOP ) {
          mTank.KeepStop();
        }
      }
      break;
    default:
      break;
  }
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
