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

ProtocolParser *mProtocol = new ProtocolParser();
#if (EM_MOTOR_SHIELD_BOARD_VERSION <= 2)
Tank mTank(mProtocol, TA_AIN1_PIN, TA_AIN2_PIN, TA_PWMA_PIN, TA_BIN1_PIN, TA_BIN2_PIN, TA_PWMB_PIN, TA_STANBY_PIN);
#else
Tank mTank(mProtocol, TA_AIN1_PIN, TA_BIN1_PIN, TA_PWMA_PIN, TA_PWMB_PIN);
#endif
byte Ps2xStatus, Ps2xType;

void setup() {
  Serial.begin(9600);
  mTank.init();
  mTank.SetBuzzerPin(TA_BUZZER_PIN);
  mTank.SetRgbPin(TA_RGB_PIN);
  mTank.Sing(S_connection);
  mTank.SetSpeed(0);
  Ps2xStatus = mTank.SetPs2xPin(TA_PS2X_CLK, TA_PS2X_CMD, TA_PS2X_CS, TA_PS2X_DAT);
  Ps2xType = mTank.mPs2x->readType();
  delay(500);
  delay(500);
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
    else if (mTank.mPs2x->Button(PSB_PAD_RIGHT)) {
      mTank.Drive(30);
    }
    else if (mTank.mPs2x->Button(PSB_PAD_LEFT)) {
      mTank.Drive(150);
    }
    else if (mTank.mPs2x->Button(PSB_PAD_DOWN)) {
      mTank.GoBack();
    }

    vibrate = mTank.mPs2x->Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
     if (mTank.mPs2x->Button(PSB_CROSS)) {             //will be TRUE if button was JUST pressed OR released
      mTank.SpeedDown(5);
    }
    else if (mTank.mPs2x->Button(PSB_TRIANGLE)) {
      mTank.SpeedUp(5);
    }
    else if (mTank.mPs2x->Button(PSB_CIRCLE)) {
      mTank.TurnRight();
    }
    else if (mTank.mPs2x->Button(PSB_SQUARE)) {
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
