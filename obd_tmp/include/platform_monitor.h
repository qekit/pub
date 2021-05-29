/*
 * @Author: your name
 * @Date: 2021-05-15 16:29:02
 * @LastEditTime: 2021-05-25 11:59:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /catkin_ws/src/obd_tmp/include/platform_monitor.h
 */
#ifndef PLATFORM_MONITOR_H_
#define PLATFORM_MONITOR_H_

#include <sys/stat.h>
#include <sys/vfs.h>
#include <unistd.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

class PlatformMonitor {
 public:
  PlatformMonitor();
  ~PlatformMonitor();

  /* CPU Info 结构体*/
  struct CpuUsageInfo_t {
    std::string name;
    unsigned int user;
    unsigned int nice;
    unsigned int system;
    unsigned int idle;
    unsigned int lowait;
    unsigned int irq;
    unsigned int softirq;
  };

  /* Memory Info  结构体*/
  struct MemInfo_t {
    unsigned long MemTotal;
    unsigned long MemFree;
    unsigned long Buffers;
    unsigned long Cached;
    unsigned long SwapCached;
  };

  void PlatformMonitorBegin();
  void PlatformMonitorThreadStart();
  void PlatformMonitorThread();
  void GetCpuUseInfo(CpuUsageInfo_t &coi);
  void CalCpuUsage(const CpuUsageInfo_t &o, const CpuUsageInfo_t &n);
  void GetMemUsage(MemInfo_t &mem);
  void GetDiskUsage();
  void GetCPUTemperature();

  /* CPU使用率 - 最后的输出结果*/
  double cpu_usage_;
  /* Mem使用率 - 最后的输出结果*/
  double mem_usage_;
  /* 指定路径的磁盘使用率 - 最后的输出结果 */
  double disk_usage_;
  /* CPU温度 - 最后输出结果 */
  double cpu_temperature_;

 private:
  /* CPU使用量 */
  CpuUsageInfo_t CpuUsageInfo1;
  CpuUsageInfo_t CpuUsageInfo2;
  /* CPU文件 */
  std::string proc_stat_path_;
  /* CPU文件读取流 */
  std::ifstream file_proc_stat_;
  /* Mem使用量 */
  MemInfo_t MemUsageInfo;
  /* CPU文件 */
  std::string proc_meminfo_path_;
  /* CPU文件读取流 */
  std::ifstream file_proc_meminfo_;
  /* 想要监控的磁盘目录（默认是个目录 “/” ） */
  std::string disk_dir_;
  /* 监控的磁盘空闲容量 */
  unsigned long long free_space_;
  /* 监控的磁盘总容量 */
  unsigned long long total_space_;
  /* 获取CPU温度的linux指令（前提：已安装lm-sensors软件） */
  std::string command_;
};
#endif  // PLATFORM_MONITOR_H_