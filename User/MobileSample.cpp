/*! @file MobileSample.cpp
 *  @version 3.3
 *  @date May 2017
 *
 *  @brief
 *  MSDK Communication STM32 example.
 *   msdk通讯stm 32的例子。
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
            //移动样品
#include "MobileSample.h"
#include "main.h"
using namespace DJI;
using namespace DJI::OSDK;
unsigned  char MSDK_data[20];
extern Vehicle  vehicle;
float gps[3];
extern Vehicle* v;

/*
void parseFromMobileCallback(DJI::OSDK::Vehicle* vehicle,DJI::OSDK::RecvContainer recvFrame,DJI::OSDK::UserData userData)
{

  uint16_t mobile_data_id;
  mobile_data_id =*(reinterpret_cast<uint16_t*>(&recvFrame.recvData.raw_ack_array));

  switch (mobile_data_id)
  {
    case 2:
			//获得多数
		
		DERROR("测试 --2--\n");
      v->obtainCtrlAuthority(controlAuthorityMobileCallback);
      break;
    case 3:
			DERROR("测试 --3--\n");
			printf("3hauhdxuha/n");
      v->releaseCtrlAuthority(controlAuthorityMobileCallback);
      break;
    case 5:
			DERROR("测试 --5--\n");
			printf("5hauhdxuha/n");
      if(v->getFwVersion() != Version::M100_31)
      {
	      v->control->action(Control::FlightCommand::startMotor,
        actionMobileCallback);
      }
      else
      {
				
        v->control->armMotors(actionMobileCallback);
      }
      break;
    case 6:
			DERROR("测试 --6--\n");
			printf("6hauhdxuha/n");
      if(v->getFwVersion() != Version::M100_31)
      {
        v->control->action(Control::FlightCommand::stopMotor,
        actionMobileCallback);
      }
      else
      {
				
	       v->control->disArmMotors(actionMobileCallback);
      }
      break;
    default:
			DERROR("测试\n");
      break;
  }
}
*/
void
parseFromMobileCallback(DJI::OSDK::Vehicle*      vehicle,
                        DJI::OSDK::RecvContainer recvFrame,
                        DJI::OSDK::UserData      userData)
{

  uint16_t mobile_data_id;
  mobile_data_id =
    *(reinterpret_cast<uint16_t*>(&recvFrame.recvData.raw_ack_array));

  switch (mobile_data_id)
  {
    case 2:
      v->obtainCtrlAuthority(controlAuthorityMobileCallback);
      break;
    case 3:
      v->releaseCtrlAuthority(controlAuthorityMobileCallback);
      break;
    case 5:
      if(v->getFwVersion() != Version::M100_31)
      {
	v->control->action(Control::FlightCommand::startMotor,
        actionMobileCallback);
      }
      else
      {
        v->control->armMotors(actionMobileCallback);
      }
      break;
    case 6:
      if(v->getFwVersion() != Version::M100_31)
      {
        v->control->action(Control::FlightCommand::stopMotor,
        actionMobileCallback);
      }
      else
      {
	v->control->disArmMotors(actionMobileCallback);
      }
      break;
    default:
      break;
  }
}

/*void parseFromMobileCallback(DJI::OSDK::Vehicle* vehicle,DJI::OSDK::RecvContainer recvFrame,DJI::OSDK::UserData userData)
{

	printf(" ---开始--- ");	
	for(int i=0;(i<recvFrame.recvInfo.len-18);i++)
	{
		MSDK_data[i]=recvFrame.recvData.raw_ack_array[i];
	  printf("%c,",recvFrame.recvData.raw_ack_array[i]);	 	
	}
	printf(" ---结束--- ");	
}*/
void controlAuthorityMobileCallback(Vehicle* vehiclePtr, RecvContainer recvFrame, DJI::OSDK::UserData userData)
{
  ACK::ErrorCode ack;
  ack.data = OpenProtocolCMD::ErrorCode::CommonACK::NO_RESPONSE_ERROR;

  unsigned char data    = 0x1;
  int           cbIndex = vehiclePtr->callbackIdIndex();

  if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <= sizeof(uint16_t))
  {
    ack.data = recvFrame.recvData.ack;
    ack.info = recvFrame.recvInfo;
  }
  else
  {
    DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
  }

  if (ack.data == OpenProtocolCMD::ErrorCode::ControlACK::SetControl::OBTAIN_CONTROL_IN_PROGRESS)
  {
    vehiclePtr->obtainCtrlAuthority(controlAuthorityMobileCallback);
  }
  else if (ack.data == OpenProtocolCMD::ErrorCode::ControlACK::SetControl::RELEASE_CONTROL_IN_PROGRESS)
  {
    vehiclePtr->releaseCtrlAuthority(controlAuthorityMobileCallback);
  }
  else
  {
    // We have a success case.   我们有个成功的例子。
    // Send this data to mobile    将此数据发送到移动
    AckReturnToMobile mobileAck;
	  //查找 外面    那个  是回调                       获取或释放
    // Find out which was called: obtain or release
    if (recvFrame.recvInfo.buf[2] == ACK::OBTAIN_CONTROL)
    {
      mobileAck.cmdID = 0x02;
    }
    else if (recvFrame.recvInfo.buf[2] == ACK::RELEASE_CONTROL)
    {
      mobileAck.cmdID = 0x03;
    }
    mobileAck.ack = static_cast<uint16_t>(ack.data);
    vehiclePtr->moc->sendDataToMSDK(reinterpret_cast<uint8_t*>(&mobileAck),sizeof(mobileAck));
  }
}

void actionMobileCallback(Vehicle* vehiclePtr, RecvContainer recvFrame,DJI::OSDK::UserData userData)
{
  ACK::ErrorCode ack;

  if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <= sizeof(uint16_t))
  {

    ack.info = recvFrame.recvInfo;

    if (vehiclePtr->getFwVersion() != Version::M100_31)
      ack.data = recvFrame.recvData.commandACK;
    else
      ack.data = recvFrame.recvData.ack;

    if (ACK::getError(ack))
      ACK::getErrorCodeMessage(ack, __func__);

    AckReturnToMobile mobileAck;
    const uint8_t     cmd[] = { recvFrame.recvInfo.cmd_set,recvFrame.recvInfo.cmd_id };

    // startMotor supported in FW version >= 3.3
    // setArm supported only on Matrice 100
    if (recvFrame.recvInfo.buf[2] == Control::FlightCommand::startMotor ||
        (memcmp(cmd, OpenProtocolCMD::CMDSet::Control::setArm, sizeof(cmd)) &&
         recvFrame.recvInfo.buf[2] == true))
    {
      mobileAck.cmdID = 0x05;
      mobileAck.ack   = static_cast<uint16_t>(ack.data);
      vehiclePtr->moc->sendDataToMSDK(reinterpret_cast<uint8_t*>(&mobileAck),
                                      sizeof(mobileAck));
    }
    else if (recvFrame.recvInfo.buf[2] == Control::FlightCommand::stopMotor ||
             (memcmp(cmd, OpenProtocolCMD::CMDSet::Control::setArm, sizeof(cmd)) &&
              recvFrame.recvInfo.buf[2] == false))
    {
			mobileAck.cmdID = 0x06;
      mobileAck.ack   = static_cast<uint16_t>(ack.data);
      vehiclePtr->moc->sendDataToMSDK(reinterpret_cast<uint8_t*>(&mobileAck),sizeof(mobileAck));
    }
  }
  else
  {
    DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
  }
}
