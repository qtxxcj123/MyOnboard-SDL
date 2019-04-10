/*! @file main.cpp
*  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  An exmaple program of DJI-onboard-SDK portable for stm32
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

#ifndef MAIN_H
#define MAIN_H

#include "Activate.h"  //激活
#include "BspUsart.h"
#include "CameraGimbalSample.h"//照相机
#include "FlightControlSample.h"//飞控
#include "MissionSample.h"
#include "MobileSample.h"
#include "Receive.h"//收到
#include "TelemetrySample.h"//遥测仪
#include "bsp.h"
#include "cppforstm32.h"
#include "dji_vehicle.hpp"//工具 飞机
#include "stm32f4xx_conf.h"
#include "timer.h"

#include "led.h"
#include "dma.h"
#include "sdio_sdcard.h"
#include "rtc.h"



extern uint32_t tick; // tick is the time stamp,which record how many ms since u
                      // initialize the system.
// warnning: after 49 days of non-reset running, tick will RESET to ZERO.



#endif // MAIN_H
