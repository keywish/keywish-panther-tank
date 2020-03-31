#include "debug.h"
#include "Emakefun_MotorDriver.h"
#include "Panther_Tank.h"
#include "ProtocolParser.h"
#include "BluetoothHandle.h"
#include "KeyMap.h"
ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol);
byte count = 0;

void setup()
{
  Serial.begin(9600);
  mTank.init(M2, M1);
  mTank.SetControlMode(E_BLUETOOTH_CONTROL);
  mTank.InitServo();
  mTank.InitRgb();
  mTank.InitBuzzer();
  mTank.SetSpeed(50);
  mTank.SetServoBaseDegree(90);
  mTank.SetServoDegree(1, 90);
  Serial.println("init ok");
  mTank.sing(S_connection);
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
            mTank.Drive(160);
            break;
          case BT_PAD_RIGHT:
            mTank.Drive(20);
            break;
          case BT_PINK:
            mTank.TurnLeftRotate();
            break;
          case BT_RED:
            mTank.TurnRightRotate();
            break;
          case BT_GREEN:
            mTank.sing(S_connection);
            mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
            mTank.SpeedUp(10);
            break;
          case BT_BLUE:
            mTank.sing(S_disconnection);
            mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
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
        mTank.SetServoDegree(mProtocol->GetServoDegreeNum(), mProtocol->GetServoDegree());
        break;
      case E_BUZZER_MODE:
        if (mProtocol->GetBuzzerMode() == E_SOUND) {
          mTank.sing(mProtocol->GetBuzzerSound());
        }
        break;
    }
    mTank.LightOff();
  }
}


void loop()
{
  static byte mode;
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
    case E_LEFT_ROTATE:
      mTank.SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT_ROTATE:
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
