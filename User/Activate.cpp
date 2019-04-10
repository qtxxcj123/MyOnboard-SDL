/*! @file Activate.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Activation process for the STM32 example App.
 *   stm 32示例应用程序的激活过程。
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
          //激活
#include "Activate.h"

extern Vehicle  vehicle;
extern Vehicle* v;
/*
APP ID      1065351
应用密钥    597ac229e16578eaa4aabb1e892b84435555d8a3693186e478f53f1f37c5d739
*/
void userActivate()
{
	//查看您的DJI开发者账户 应用程序和应用程序ID
  //! At your DJI developer account look for: app_key and app ID

//  //static char key_buf[65] = "your app_key here";
//  static char key_buf[65] = "597ac229e16578eaa4aabb1e892b84435555d8a3693186e478f53f1f37c5d739";
//  DJI::OSDK::Vehicle::ActivateData user_act_data;
//  //user_act_data.ID = 0000; /*your app ID here*/
//  user_act_data.ID = 1065351; /*your app ID here*/
//  user_act_data.encKey = key_buf;
//   //  激活
//  v->activate(&user_act_data);
	
	static char key_buf[65] = "4542358bb38d7ba447a10abbce94a0fae352cb8b1019d13dcce5097bd2ed11c0";

  DJI::OSDK::Vehicle::ActivateData user_act_data;
  user_act_data.ID = 1065690; /*your app ID here*/

  user_act_data.encKey = key_buf;

  v->activate(&user_act_data);
}
