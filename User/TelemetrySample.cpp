/*! @file telemetry_sample.cpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  Telemetry API usage in a Linux environment.   ���߻����е�ң��apiʹ�á�
 *  Shows example usage of the new data subscription API.    չʾ�������ݶ���api��ʾ���÷���
 *
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
           //ң����
#include "TelemetrySample.h"

extern Vehicle  vehicle;
extern Vehicle* v;

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

bool
getBroadcastData()
{
  // Counters ������
  int elapsedTimeInMs = 0;
  int timeToPrintInMs = 2000;

  // We will listen to five broadcast data sets:  ���ǻ���������㲥���ݼ���
  // 1. Flight Status   ����״̬
  // 2. Global Position  ȫ���λ
  // 3. RC Channels   rcƵ��
  // 4. Velocity   �ٶ�
  // 5. Quaternion  ��Ԫ��

  // Please make sure your drone is in simulation mode. You can  ��ȷ���������˻�����ģ��ģʽ�������
  // fly the drone with your RC to get different values.  �����rc��ʻ���˻��õ���ͬ��ֵ��

  Telemetry::Status         status;  //ң�⣺״����
  Telemetry::GlobalPosition globalPosition;  //ң�⣺ȫ��λ��
  Telemetry::RC             rc;  //ң�⣺:rc RC��
  Telemetry::Vector3f       velocity;   //ң�⣺����3f�ٶȣ�
  Telemetry::Quaternion     quaternion;  //ң�⣺:��Ԫ����Ԫ����

  const int TIMEOUT = 20;  //��ʱ

  // Re-set Broadcast frequencies to their default values  ���㲥Ƶ����������ΪĬ��ֵ
  ACK::ErrorCode ack = v->broadcast->setBroadcastFreqDefaults(TIMEOUT);

  // Print in a loop for 2 seconds  ѭ����ӡ2����
  while (elapsedTimeInMs < timeToPrintInMs)
  {
    // Matrice 100 broadcasts only flight status  Matrice100ֻ���ŷ���״̬
    status         = v->broadcast->getStatus();
    globalPosition = v->broadcast->getGlobalPosition();
    rc             = v->broadcast->getRC();
    velocity       = v->broadcast->getVelocity();
    quaternion     = v->broadcast->getQuaternion();

    printf("Counter = %d:\n", elapsedTimeInMs);
    printf("-------\n");
    printf("Flight Status = %d\n", (unsigned)status.flight);
    printf("Position (LLA) = %.3f, %.3f, %.3f\n", globalPosition.latitude,
           globalPosition.longitude, globalPosition.altitude);
    printf("RC Commands (r/p/y/thr) = %d, %d, %d, %d\n", rc.roll, rc.pitch,
           rc.yaw, rc.throttle);
    printf("Velocity (vx,vy,vz) = %.3f, %.3f, %.3f\n", velocity.x, velocity.y,
           velocity.z);
    printf("Attitude Quaternion (w,x,y,z) = %.3f, %.3f, %.3f, %.3f\n",
           quaternion.q0, quaternion.q1, quaternion.q2, quaternion.q3);
    printf("-------\n\n");

    elapsedTimeInMs += 5;
  }

  printf("Done printing!\n");
  return true;
}



bool
subscribeToData()
{

  // Counters   ������
  int elapsedTimeInMs = 0;   //���ʱ��
  int timeToPrintInMs = 4000;

  // We will subscribe to six kinds of data:   ���ǽ������������ݣ�
  // 1. Flight Status at 1 Hz   ����״̬Ϊ1Сʱ
  // 2. Fused Lat/Lon at 10Hz    10Сʱ�ۻ�
  // 3. Fused Altitude at 10Hz   10hz�����ڸ߶�
  // 4. RC Channels at 50 Hz      rcƵ��50����
  // 5. Velocity at 50 Hz        �ٶ���50����
  // 6. Quaternion at 200 Hz     ��Ԫ��Ϊ200����

  // Package 0: Subscribe to flight status at freq 1 Hz   �ײ�0����frq 1hz���ķ���״̬
  int       pkgIndex        = 0;
  int       freq            = 1;
  TopicName topicList1Hz[]  = { TOPIC_STATUS_FLIGHT };
  int       numTopic        = sizeof(topicList1Hz) / sizeof(topicList1Hz[0]);
	           //ʱ���
  bool      enableTimestamp = false;
     //pkg״̬        ����       ��ʼ���׼���������һ
  bool pkgStatus = v->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList1Hz, enableTimestamp, freq);
  if (!(pkgStatus))
  {
    return pkgStatus;
  }

  v->subscribe->startPackage(pkgIndex);
  delay_nms(500);
  /*ack = waitForACK();
  if(ACK::getError(ack))
  {
    ACK::getErrorCodeMessage(ack, func);

    // Cleanup
    v->subscribe->removePackage(pkgIndex);
    ack = waitForACK();
    if(ACK::getError(ack))
    {
      ACK::getErrorCodeMessage(ack, func);
    }

    return false;
  }*/

  // Package 1: Subscribe to Lat/Lon, and Alt at freq 10 Hz   �ײ�1������late/lon������10hz��Ѷ���
  pkgIndex                  = 1;
  freq                      = 10;
  TopicName topicList10Hz[] = { TOPIC_GPS_FUSED };
  numTopic                  = sizeof(topicList10Hz) / sizeof(topicList10Hz[0]);
  enableTimestamp           = false;

  pkgStatus = v->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList10Hz, enableTimestamp, freq);
  if (!(pkgStatus))
  {
    return pkgStatus;
  }

  v->subscribe->startPackage(pkgIndex);
  delay_nms(500);
  /*ack = waitForACK();
  if(ACK::getError(ack))
  {
    ACK::getErrorCodeMessage(ack, func);

    // Cleanup
    v->subscribe->removePackage(pkgIndex);
    ack = waitForACK();
    if(ACK::getError(ack))
    {
      ACK::getErrorCodeMessage(ack, func);
    }

    return false;
  }*/

  // Package 2: Subscribe to RC Channel and Velocity at freq 50 Hz   ��װ2������rcƵ�����ٶ�Ϊ50����
  pkgIndex                  = 2;
  freq                      = 50;
  TopicName topicList50Hz[] = { TOPIC_RC, TOPIC_VELOCITY };
  numTopic                  = sizeof(topicList50Hz) / sizeof(topicList50Hz[0]);
  enableTimestamp           = false;

  pkgStatus = v->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList50Hz, enableTimestamp, freq);
  if (!(pkgStatus))
  {
    return pkgStatus;
  }

  v->subscribe->startPackage(pkgIndex);
  delay_nms(500);
  /*ack = waitForACK();
  if(ACK::getError(ack))
  {
    ACK::getErrorCodeMessage(ack, func);

    // Cleanup
    v->subscribe->removePackage(pkgIndex);
    ack = waitForACK();
    if(ACK::getError(ack))
    {
      ACK::getErrorCodeMessage(ack, func);
    }

    return false;
  }*/

  // Package 3: Subscribe to Quaternion at freq 200 Hz.     ��װ3��������Ԫ�������200hz��
  pkgIndex                   = 3;
  freq                       = 200;
  TopicName topicList200Hz[] = { TOPIC_QUATERNION };
  numTopic        = sizeof(topicList200Hz) / sizeof(topicList200Hz[0]);
  enableTimestamp = false;

  pkgStatus = v->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList200Hz, enableTimestamp, freq);
  if (!(pkgStatus))
  {
    return pkgStatus;
  }

  v->subscribe->startPackage(pkgIndex);
  delay_nms(500);
  /*ack = waitForACK();
  if(ACK::getError(ack))
  {
    ACK::getErrorCodeMessage(ack, func);

    // Cleanup
    v->subscribe->removePackage(pkgIndex);
    ack = waitForACK();
    if(ACK::getError(ack))
    {
      ACK::getErrorCodeMessage(ack, func);
    }

    return false;
  }*/

  // Wait for the data to start coming in. �ȴ����ݿ�ʼ����
  delay_nms(8000);

  // Get all the data once before the loop to initialize vars  ��ѭ��֮ǰ��ȡ���������Գ�ʼ��VAR
  TypeMap<TOPIC_STATUS_FLIGHT>::type flightStatus;  //���ͣ����ͣ�����״̬��
  TypeMap<TOPIC_GPS_FUSED>::type     latLon;   //���ͣ����ͣ�latlon��
  TypeMap<TOPIC_RC>::type            rc;   //���ͣ�RC�ͣ�
  TypeMap<TOPIC_VELOCITY>::type      velocity;  //���ͣ����ͣ��ٶȣ�
  TypeMap<TOPIC_QUATERNION>::type    quaternion;  //���ͣ���Ԫ����

  uint32_t PRINT_TIMEOUT = 4000; //��ӡʱ��=4000;/����/���� // milliseconds  
  uint32_t RETRY_TICK    = 500;  //����_tick=500;/���� milliseconds
  uint32_t nextRetryTick = 0;    //��ֵ=0;//ǧ��  millisesonds
  uint32_t timeoutTick; //��ʱ
  //��ʱ             Э���        Ŀ����������                  ��ӡ��ʱ
  timeoutTick = v->protocolLayer->getDriver()->getTimeStamp() + PRINT_TIMEOUT;
  do
  {
		//����״̬        ����        ���ֵ   ����״̬
    flightStatus = v->subscribe->getValue<TOPIC_STATUS_FLIGHT>();
		// ����                                   gps_�ں�
    latLon       = v->subscribe->getValue<TOPIC_GPS_FUSED>();
    rc           = v->subscribe->getValue<TOPIC_RC>();
    velocity     = v->subscribe->getValue<TOPIC_VELOCITY>(); //�ٶ�
    quaternion   = v->subscribe->getValue<TOPIC_QUATERNION>(); //��Ԫ��

    printf("\n\nCounter = %d:\n", elapsedTimeInMs);
		printf("\n\n��ʼ\n\n");
    printf("\n\nFlight Status = %d\n", (int)flightStatus);
    printf("\n\nPosition (LLA) = %.3f, %.3f, %.3f\n", latLon.latitude,latLon.longitude, latLon.altitude);
    printf("\n\nRC Commands (r/p/y/thr) = %d, %d, %d, %d\n", rc.roll, rc.pitch,rc.yaw, rc.throttle);
    printf("\n\nVelocity (vx,vy,vz) = %.3f, %.3f, %.3f\n", velocity.data.x,velocity.data.y, velocity.data.z);
    printf("\n\nAttitude Quaternion (w,x,y,z) = %.3f, %.3f, %.3f, %.3f\n",quaternion.q0, quaternion.q1, quaternion.q2, quaternion.q3);
		printf("\n\n����\n\n");
		printf("\n\r");
		

    delay_nms(500);
    nextRetryTick = v->protocolLayer->getDriver()->getTimeStamp() + RETRY_TICK;
  } while (nextRetryTick < timeoutTick);

  printf("Done printing!\n"); //ӡˢ���
	printf("------���------\n"); //ӡˢ���
  v->subscribe->removePackage(0);  //�ƶ��׼�
  delay_nms(3000);
  v->subscribe->removePackage(1);
  delay_nms(3000);
  v->subscribe->removePackage(2);
  delay_nms(3000);
  v->subscribe->removePackage(3);
  delay_nms(3000);

  return true;
}