/*
 * @Author: your name
 * @Date: 2021-05-15 15:35:35
 * @LastEditTime: 2021-05-24 18:52:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /catkin_ws/src/obd_tmp/src/main.cpp
 */
#include "platform_monitor.h"

int main(int argc, char* argv[]) {
  PlatformMonitor pm;
  pm.PlatformMonitorBegin();
  getchar();
  //pm.GetDiskUsage();
  //pm.GetCPUTemperature();
  return 0;
}