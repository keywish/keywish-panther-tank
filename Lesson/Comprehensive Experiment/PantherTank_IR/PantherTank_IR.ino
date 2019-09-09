#include<Arduino.h>
#include<Wire.h>
#include "Panther_Tank.h"
#include "ProtocolParser.h"
#include "BluetoothHandle.h"
#include "debug.h"
#include "KeyMap.h"
ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol, 1, 2);
void setup()
{
  Serial.begin(9600);
  mTank.SetControlMode(E_INFRARED_REMOTE_CONTROL);
  mTank.SetBatteryCheckPin(BATTERY_PIN);
  mTank.InitIrPin();
  mTank.InitServoPin();
  mTank.InitRgbPin();
  mTank.InitBuzzerPin();
  mTank.SetSpeed(60);
  mTank.init();
  mTank.SetServoBaseDegree(90);
  mTank.SetServoDegree(1, 90);
}

void HandleInfaredRemote (byte Irkey)
{
  switch (Irkey) {
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
}

void loop()
{
  static bool recv_flag;
  mProtocol->RecevData();
  if (recv_flag = mProtocol->ParserPackage()) {
    if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
      mTank.SetControlMode(mProtocol->GetControlMode());
      return;
    }
  }
  switch (mTank.GetControlMode()) {
    case E_INFRARED_REMOTE_CONTROL:
      byte Irkey;
      Irkey = mTank.GetIrKey();
      if (Irkey != NULL)
      {
        HandleInfaredRemote(Irkey);
        delay(110);
      } else
        mTank.KeepStop();
      break;
    default:
      break;
  }
  switch (mTank.GetStatus()) {
    case E_FORWARD:
      mTank.SetRgbColor(E_RGB_ALL, RGB_WHITE);
      break;
    case E_LEFT:
      mTank.SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT:
      mTank.SetRgbColor(E_RGB_RIGHT, RGB_WHITE);
      break;
    case E_BACK:
      mTank.SetRgbColor(E_RGB_ALL, RGB_RED);
      break;
    case E_STOP:
      mTank.LightOff();
      break;
    default:
      break;
  }
}
