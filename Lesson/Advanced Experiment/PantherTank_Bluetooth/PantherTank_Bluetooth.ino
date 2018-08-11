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
#define BIN1_PIN 4
#define BIN2_PIN 2
#define PWMB_PIN 6
#define STANBY_PIN 7
#define BUZZER_PIN 9
#define RGB_PIN A3
#define ECHO_PIN A0
#define TRIG_PIN A1
#define SERVO_PIN 13
#define PS2X_CLK A4
#define PS2X_CMD A1
#define PS2X_CS  A2
#define PS2X_DAT A0
#define IR_PIN 8
#define UL_LIMIT_MIN 50
#define UL_LIMIT_MID 40
#define UL_LIMIT_MAX 2000

ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol, TA_AIN1_PIN, TA_AIN2_PIN, TA_PWMA_PIN, BIN1_PIN, BIN2_PIN, PWMB_PIN, STANBY_PIN);
byte Ps2xStatus, Ps2xType;
ST_PROTOCOL SendData;

void setup() {
  Serial.begin(9600);
  mTank.init();
  mTank.SetControlMode(E_BLUTOOTH_CONTROL);
  mTank.SetBuzzerPin(BUZZER_PIN);
  mTank.SetRgbPin(RGB_PIN);
  mTank.Sing(S_connection);
  mTank.SetSpeed(100);
  mTank.SetUltrasonicPin(TRIG_PIN, ECHO_PIN, SERVO_PIN);
  mTank.mUltrasonic->SetServoBaseDegree(88);
  mTank.mUltrasonic->SetServoDegree(90);
  delay(500);
  mTank.SetIrPin(IR_PIN);
  //Infrared Tracing pins conflig with Ps2x pins
  //mTank.SetInfraredTracingPin(TA_INFRARED_TRACING_PIN1, TA_INFRARED_TRACING_PIN2, TA_INFRARED_TRACING_PIN3);
  //delay(1000);  //added delay to give wireless ps2 module some time to startup, before configuring it
  //Ps2xStatus = mTank.SetPs2xPin(PS2X_CLK, PS2X_CMD, PS2X_CS, PS2X_DAT);
  //Ps2xType = mTank.mPs2x->readType();
}

void HandleBloothRemote()
  {
  if (mProtocol->ParserPackage()) {
    switch (mProtocol->GetRobotControlFun()) {
      case E_INFO:
        break;
      case E_ROBOT_CONTROL_DIRECTION:
        //mTank.SetDirection(mProtocol->GetRobotDegree());
        mTank.Drive(mProtocol->GetRobotDegree());
        break;
      case E_ROBOT_CONTROL_SPEED:
        mTank.SetSpeed(mProtocol->GetRobotSpeed());
        break ;
      case E_CONTROL_MODE:
        mTank.SetControlMode(mProtocol->GetControlMode());
        break;
      case E_BUZZER:
        // mTank.Sing();
        // mTank.PianoSing(mProtocol->GetPianoSing());
        break;
      case E_VERSION:
        break;
    }
   }
  }

  void HandleInfaredRemote(byte irKeyCode)
  {
  switch ((E_IR_KEYCODE)mTank.mIrRecv->getIrKey(irKeyCode)) {
    case IR_KEYCODE_STAR:
      mTank.SpeedUp(10);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "mTank.Speed = %d \n", mTank.Speed);
      break;
    case IR_KEYCODE_POUND:
      DEBUG_LOG(DEBUG_LEVEL_INFO, " start Degree = %d \n", mTank.Degree);
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
      mTank.TurnLeft();
      break;
    case IR_KEYCODE_RIGHT:
      mTank.TurnRight();
      break;
    default:
      break;
  }
  //  mTank.mIrRecv->resume();
  }

  void HandleInfraredTracing(void)
  {
  switch (mTank.mInfraredTracing->getValue()) {
    case IT_ALL_BLACK:
    case IT_ALL_WHITE:
      mTank.KeepStop();
      break;
    case IT_CENTER:
      mTank.SetSpeed(100);
      mTank.GoForward();
      break;
    case IT_RIGHT1:
      mTank.SetSpeed(80);
      mTank.TurnRight();
      break;
    case IT_LEFT1:
      mTank.SetSpeed(80);
      mTank.TurnLeft();
      break;
  }
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
      mTank.TurnRight();
    }
    if (mTank.mPs2x->Button(PSB_PAD_LEFT)) {
      mTank.TurnLeft();
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
      mTank.Drive(0);
    }
    if (mTank.mPs2x->Button(PSB_SQUARE)) {
      mTank.Drive(180);
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

void HandleUltrasonicAvoidance()
{
    uint16_t UlFrontDistance,UlLeftDistance,UlRightDistance;
    UlFrontDistance =  mTank.mUltrasonic->GetUltrasonicFrontDistance();
    if ((UlFrontDistance > UL_LIMIT_MID) && (UlFrontDistance < UL_LIMIT_MAX))
    {
        mTank.SetSpeed(100);
        mTank.GoForward();
    }
    else if ((UlFrontDistance < UL_LIMIT_MIN) || (UlFrontDistance > UL_LIMIT_MAX))
    {
        mTank.KeepStop();
        UlRightDistance = mTank.mUltrasonic->GetUltrasonicRightDistance();
        UlLeftDistance = mTank.mUltrasonic->GetUltrasonicLeftDistance();
        if((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX) && (UlRightDistance > UlLeftDistance))
        {
            mTank.SetSpeed(80);
            mTank.TurnRight();
            delay(200);
        }
        else if((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX) && (UlLeftDistance > UlRightDistance))
        {
            mTank.SetSpeed(80);
            mTank.TurnLeft();
            delay(200);
        }
        else if((UlRightDistance < UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MIN) )
        {
            mTank.SetSpeed(80);
            mTank.Drive(0);
            delay(200);
        }
    }
}

void SendTracingSignal(){
    byte TracingSignal = mTank.mInfraredTracing->getValue();
    SendData.start_code = 0xAA;
    SendData.type = 0x01;
    SendData.addr = 0x01;
    SendData.function = E_INFRARED_TRACKING;
    SendData.data = (byte *)&TracingSignal;
    SendData.len = 7;
    SendData.end_code = 0x55;
    mProtocol->SendPackage(&SendData, 1);
}

void SendUltrasonicData(){
    unsigned int UlFrontDistance =  mTank.mUltrasonic->GetUltrasonicFrontDistance();
    SendData.start_code = 0xAA;
    SendData.type = 0x01;
    SendData.addr = 0x01;
    SendData.function = E_ULTRASONIC_AVOIDANCE;
    SendData.data = (byte *)&UlFrontDistance;
    SendData.len = 7;
    SendData.end_code = 0x55;
    mProtocol->SendPackage(&SendData, 1);
}

void loop() {
  mProtocol->RecevData();
  if (mTank.GetControlMode() !=  E_BLUTOOTH_CONTROL) {
    if (mProtocol->ParserPackage()) {
        if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
        mTank.SetControlMode(mProtocol->GetControlMode());
       }
    }
  }
  switch (mTank.GetControlMode()) {
    case E_BLUTOOTH_CONTROL:
      HandleBloothRemote();
      // DEBUG_LOG(DEBUG_LEVEL_INFO, "E_BLUTOOTH_CONTROL \n");
      break;
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
    case E_INFRARED_TRACKING_MODE:
      // DEBUG_LOG(DEBUG_LEVEL_INFO, "E_INFRARED_TRACKING \n");
      mTank.SetInfraredTracingPin(TA_INFRARED_TRACING_PIN1, TA_INFRARED_TRACING_PIN2, TA_INFRARED_TRACING_PIN3);
      HandleInfraredTracing();
      break;
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
    case E_ULTRASONIC_AVOIDANCE:
      HandleUltrasonicAvoidance();
      SendUltrasonicData();
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
