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
#define PS2X_CLK A4
#define PS2X_CMD A1
#define PS2X_CS  A2
#define PS2X_DAT A0

ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol, TA_AIN1_PIN, TA_AIN2_PIN, TA_PWMA_PIN, BIN1_PIN, BIN2_PIN, PWMB_PIN, STANBY_PIN);
byte Ps2xStatus, Ps2xType;

void setup() {
  Serial.begin(9600);
  mTank.init();
  mTank.SetControlMode(E_PS2_REMOTE_CONTROL);
  mTank.SetBuzzerPin(BUZZER_PIN);
  mTank.SetRgbPin(RGB_PIN);
  mTank.Sing(S_connection);
  mTank.SetSpeed(0);
  delay(1000);  
  Ps2xStatus = mTank.SetPs2xPin(PS2X_CLK, PS2X_CMD, PS2X_CS, PS2X_DAT);
  Ps2xType = mTank.mPs2x->readType();
}

void HandlePs2xRemote()
{
  static int vibrate = 0;
  byte PSS_X = 0, PSS_Y = 0;
  mTank.mPs2x->read_gamepad(false, vibrate); // read controller and set large motor to spin at 'vibrate' speed
  if (mTank.mPs2x->ButtonDataByte()) {
    if (mTank.mPs2x->Button(PSB_PAD_UP)) {     //will be TRUE as long as button is pressed
      mTank.GoForward();
    }
    if (mTank.mPs2x->Button(PSB_PAD_RIGHT)) {
      mTank.Drive(30);
    }
    if (mTank.mPs2x->Button(PSB_PAD_LEFT)) {
      mTank.Drive(150);
    }
    if (mTank.mPs2x->Button(PSB_PAD_DOWN)) {
      mTank.GoBack();
    }

    vibrate = mTank.mPs2x->Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (mTank.mPs2x->Button(PSB_CROSS)) {             //will be TRUE if button was JUST pressed OR released
      mTank.SpeedDown(5);
    }
    if (mTank.mPs2x->Button(PSB_TRIANGLE)) {
      mTank.SpeedUp(5);
    }
    if (mTank.mPs2x->Button(PSB_CIRCLE)) {
      mTank.TurnRight();
    }
    if (mTank.mPs2x->Button(PSB_SQUARE)) {
      mTank.TurnLeft();
    }
    if (mTank.mPs2x->Button(PSB_L1) || mTank.mPs2x->Button(PSB_R1)) {
      PSS_X = mTank.mPs2x->Analog(PSS_LY);
      PSS_Y = mTank.mPs2x->Analog(PSS_LX);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "%x %x ", PSS_X, PSS_Y);
      mTank.mRgb->setColor(E_RGB_LEFT, PSS_X, PSS_Y, PSS_X + PSS_Y);
      PSS_X = mTank.mPs2x->Analog(PSS_RY);
      PSS_Y = mTank.mPs2x->Analog(PSS_RX);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "%x %x\n", PSS_X, PSS_Y);
      mTank.mRgb->setColor(E_RGB_RIGHT, PSS_X, PSS_Y, PSS_X + PSS_Y);
      mTank.mRgb->show();
    }
  } else {
    mTank.KeepStop();
  }
  delay(50);
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
    case E_PS2_REMOTE_CONTROL:
      while (Ps2xStatus != 0) { //skip loop if no controller found
        delay(500);
        Ps2xStatus = mTank.ResetPs2xPin();
        Ps2xType = mTank.mPs2x->readType();
        //DEBUG_LOG(DEBUG_LEVEL_INFO, "ps2x reconnect status = %d, type = %d\n", Ps2xStatus, Ps2xType);
      }
      if (Ps2xType != 2) {
        // Guitar Hero Controller
        HandlePs2xRemote();
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
