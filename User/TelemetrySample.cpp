/*! @file telemetry_sample.cpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  Telemetry API usage in a Linux environment.   在线环境中的遥测api使用。
 *  Shows example usage of the new data subscription API.    展示了新数据订阅api的示例用法。
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
           //遥测仪
#include "TelemetrySample.h"

extern Vehicle  vehicle;
extern Vehicle* v;

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

bool
getBroadcastData()
{
  // Counters 计数器
  int elapsedTimeInMs = 0;
  int timeToPrintInMs = 2000;

  // We will listen to five broadcast data sets:  我们会收听五个广播数据集：
  // 1. Flight Status   飞行状态
  // 2. Global Position  全球地位
  // 3. RC Channels   rc频道
  // 4. Velocity   速度
  // 5. Quaternion  四元数

  // Please make sure your drone is in simulation mode. You can  请确保您的无人机处于模拟模式。你可以
  // fly the drone with your RC to get different values.  用你的rc驾驶无人机得到不同的值。

  Telemetry::Status         status;  //遥测：状况；
  Telemetry::GlobalPosition globalPosition;  //遥测：全球定位；
  Telemetry::RC             rc;  //遥测：:rc RC；
  Telemetry::Vector3f       velocity;   //遥测：向量3f速度；
  Telemetry::Quaternion     quaternion;  //遥测：:四元数四元数；

  const int TIMEOUT = 20;  //超时

  // Re-set Broadcast frequencies to their default values  将广播频率重新设置为默认值
  ACK::ErrorCode ack = v->broadcast->setBroadcastFreqDefaults(TIMEOUT);

  // Print in a loop for 2 seconds  循环打印2秒钟
  while (elapsedTimeInMs < timeToPrintInMs)
  {
    // Matrice 100 broadcasts only flight status  Matrice100只播放飞行状态
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

  // Counters   计数器
  int elapsedTimeInMs = 0;   //间隔时间
  int timeToPrintInMs = 4000;

  // We will subscribe to six kinds of data:   我们将订阅六种数据：
  // 1. Flight Status at 1 Hz   飞行状态为1小时
  // 2. Fused Lat/Lon at 10Hz    10小时熔化
  // 3. Fused Altitude at 10Hz   10hz的熔融高度
  // 4. RC Channels at 50 Hz      rc频道50赫兹
  // 5. Velocity at 50 Hz        速度在50赫兹
  // 6. Quaternion at 200 Hz     四元数为200赫兹

  // Package 0: Subscribe to flight status at freq 1 Hz   套餐0：在frq 1hz订阅飞行状态
  int       pkgIndex        = 0;
  int       freq            = 1;
  TopicName topicList1Hz[]  = { TOPIC_STATUS_FLIGHT };
  int       numTopic        = sizeof(topicList1Hz) / sizeof(topicList1Hz[0]);
	           //时间戳
  bool      enableTimestamp = false;
     //pkg状态        订阅       初始化套件从排名第一
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

  // Package 1: Subscribe to Lat/Lon, and Alt at freq 10 Hz   套餐1：订阅late/lon，并在10hz免费订阅
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

  // Package 2: Subscribe to RC Channel and Velocity at freq 50 Hz   包装2：订阅rc频道，速度为50赫兹
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

  // Package 3: Subscribe to Quaternion at freq 200 Hz.     包装3：订阅四元数在免费200hz。
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

  // Wait for the data to start coming in. 等待数据开始进入
  delay_nms(8000);

  // Get all the data once before the loop to initialize vars  在循环之前获取所有数据以初始化VAR
  TypeMap<TOPIC_STATUS_FLIGHT>::type flightStatus;  //类型：类型：飞行状态；
  TypeMap<TOPIC_GPS_FUSED>::type     latLon;   //类型：类型：latlon；
  TypeMap<TOPIC_RC>::type            rc;   //类型：RC型；
  TypeMap<TOPIC_VELOCITY>::type      velocity;  //类型：类型：速度；
  TypeMap<TOPIC_QUATERNION>::type    quaternion;  //类型：四元数；

  uint32_t PRINT_TIMEOUT = 4000; //打印时间=4000;/毫秒/毫秒 // milliseconds  
  uint32_t RETRY_TICK    = 500;  //重置_tick=500;/毫秒 milliseconds
  uint32_t nextRetryTick = 0;    //极值=0;//千秒  millisesonds
  uint32_t timeoutTick; //超时
  //超时             协议层        目标驱动程序                  打印超时
  timeoutTick = v->protocolLayer->getDriver()->getTimeStamp() + PRINT_TIMEOUT;
  do
  {
		//飞行状态        订阅        获得值   飞行状态
    flightStatus = v->subscribe->getValue<TOPIC_STATUS_FLIGHT>();
		// 经度                                   gps_融合
    latLon       = v->subscribe->getValue<TOPIC_GPS_FUSED>();
    rc           = v->subscribe->getValue<TOPIC_RC>();
    velocity     = v->subscribe->getValue<TOPIC_VELOCITY>(); //速度
    quaternion   = v->subscribe->getValue<TOPIC_QUATERNION>(); //四元数

    printf("\n\nCounter = %d:\n", elapsedTimeInMs);
		printf("\n\n开始\n\n");
    printf("\n\nFlight Status = %d\n", (int)flightStatus);
    printf("\n\nPosition (LLA) = %.3f, %.3f, %.3f\n", latLon.latitude,latLon.longitude, latLon.altitude);
    printf("\n\nRC Commands (r/p/y/thr) = %d, %d, %d, %d\n", rc.roll, rc.pitch,rc.yaw, rc.throttle);
    printf("\n\nVelocity (vx,vy,vz) = %.3f, %.3f, %.3f\n", velocity.data.x,velocity.data.y, velocity.data.z);
    printf("\n\nAttitude Quaternion (w,x,y,z) = %.3f, %.3f, %.3f, %.3f\n",quaternion.q0, quaternion.q1, quaternion.q2, quaternion.q3);
		printf("\n\n结束\n\n");
		printf("\n\r");
		

    delay_nms(500);
    nextRetryTick = v->protocolLayer->getDriver()->getTimeStamp() + RETRY_TICK;
  } while (nextRetryTick < timeoutTick);

  printf("Done printing!\n"); //印刷完成
	printf("------完成------\n"); //印刷完成
  v->subscribe->removePackage(0);  //移动套件
  delay_nms(3000);
  v->subscribe->removePackage(1);
  delay_nms(3000);
  v->subscribe->removePackage(2);
  delay_nms(3000);
  v->subscribe->removePackage(3);
  delay_nms(3000);

  return true;
}