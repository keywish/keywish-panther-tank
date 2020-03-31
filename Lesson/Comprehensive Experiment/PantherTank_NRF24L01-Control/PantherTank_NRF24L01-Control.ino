
#include "Panther_Tank.h"
#include "ProtocolParser.h"
#include "BluetoothHandle.h"
#include "debug.h"
#include "KeyMap.h"

ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol);

void setup()
{
  Serial.begin(9600);
  mTank.init(M2, M1);
  mTank.SetControlMode(E_NRF24L01_CONTROL);
  mTank.InitServo();
  mTank.InitRgb();
  mTank.InitBuzzer();
  mTank.SetSpeed(50);
  mTank.SetServoBaseDegree(90);
  mTank.SetServoDegree(1, 90);
  mTank.InitNrf24L01("Tank");
  Serial.println("init ok");
  mTank.sing(S_connection);
}

void HandleNrf24L01Remote()
{
  byte value[12] = {0};
  bool godata = false;
  while (mTank.Nrf24L01->dataReady()) { //When the program is received, the received data is output from the serial port
    mTank.Nrf24L01->getData(value);
    mProtocol->RecevData(value, 12);
    /*  for (int i = 0; i < 12; i++) {
         Serial.print(value[i], HEX); Serial.print(" ");
      }
         Serial.println("");
    */
  }
  if (mProtocol->ParserPackage())
  {
    Serial.println("got rf package");
    E_CONTOROL_FUNC fun = mProtocol->GetRobotControlFun();
    if ( fun == E_ROBOT_CONTROL_DIRECTION) {
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ROBOT_CONTROL_DIRECTION \n");
      int degree = mProtocol->GetRobotDegree();
      mTank.Drive(degree);
    }
    if (fun == E_ROBOT_CONTROL_SPEED) {
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ROBOT_CONTROL_SPEED \n");
      mTank.SetSpeed(mProtocol->GetRobotSpeed());
    }
    if (fun == E_BUTTON) {
      switch (mProtocol->GetBluetoothButton()) {
        case BT_L1:
          mTank.SpeedDown(5);
          mTank.sing(S_disconnection);
          mTank.SetRgbColor(E_RGB_ALL, RGB_BLUE);
          break;
        case BT_R1:
          mTank.SpeedUp(5);
          mTank.sing(S_connection);
          mTank.SetRgbColor(E_RGB_ALL, RGB_GREEN);
          break;
        case BT_PAD_UP:
          mTank.GoForward();
          break;
        case BT_PAD_DOWN:
          mTank.GoBack();
          break;
        case BT_PAD_LEFT:
          mTank.TurnLeftRotate();
          break;
        case BT_PAD_RIGHT:
          mTank.TurnRightRotate();
          break;
        default:
          break;
      }
    }
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
    case E_NRF24L01_CONTROL:
      HandleNrf24L01Remote();
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
