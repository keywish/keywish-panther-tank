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
  mTank.SetControlMode(E_BLUETOOTH_CONTROL);//E_PS2_REMOTE_CONTROL//E_BLUETOOTH_CONTROL//E_ULTRASONIC_FOLLOW_MODE//E_ULTRASONIC_AVOIDANCE
  mTank.SetBatteryCheckPin(BATTERY_PIN);
  mTank.InitServoPin();
  mTank.InitRgbPin();
  mTank.InitBuzzerPin();
  mTank.SetSpeed(100);
  mTank.init();
  mTank.SetServoBaseDegree(90);
  mTank.SetServoDegree(1, 90);
}

void HandleBluetoothRemote(bool recv_flag)
{
  if (recv_flag) {
    
    switch (mProtocol->GetRobotControlFun()) {
      case E_BUTTON:
        switch (mProtocol->GetBluetoothButton())
        {
          case BT_PAD_UP:
            mTank.GoForward();
            break;
          case BT_PAD_DOWN:
            mTank.GoBack();
            break;
          case BT_PAD_LEFT:
            mTank.TurnLeft();
            break;
          case BT_PAD_RIGHT:
            mTank.TurnRight();
            break;
          case BT_L3:
            mTank.SpeedUp(10);
            break;
          case BT_R3:
            mTank.SpeedDown(10);
            break;
        }
        break;
      case E_ROBOT_CONTROL_DIRECTION:
        mTank.Drive(mProtocol->GetRobotDegree());
        break;
      case E_ROBOT_CONTROL_SPEED:
        mTank.SetSpeed(mProtocol->GetRobotSpeed());
        break ;
      case E_CONTROL_MODE:
        mTank.SetControlMode(mProtocol->GetControlMode());
        break;
      case E_LED:
        mTank.SetRgbColor(E_RGB_ALL, mProtocol->GetRgbValue());
        break;
      case E_SERVER_DEGREE:
        mTank.SetServoDegree(1, mProtocol->GetServoDegree());
        break;
    }
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
    case E_BLUETOOTH_CONTROL:
      HandleBluetoothRemote(recv_flag);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_BLUETOOTH_CONTROL \n");
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
    case E_SPEED_UP:
      mTank.sing(S_connection);
      mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
      break;
    case E_SPEED_DOWN:
      mTank.sing(S_disconnection);
      mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
      break;
    default:
      break;
  }
}
