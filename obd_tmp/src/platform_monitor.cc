#include "platform_monitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

PlatformMonitor::PlatformMonitor()
    : proc_stat_path_("/proc/stat"),
      cpu_usage_(0),
      proc_meminfo_path_("/proc/meminfo"),
      disk_dir_("/"),
      command_("sensors"){};

PlatformMonitor::~PlatformMonitor(){};

/**
 * @description:平台监控起始函数
 * @param {*}
 * @return {*}
 */
void PlatformMonitor::PlatformMonitorBegin() { PlatformMonitorThreadStart(); }

/**
 * @description:创建平台监控线程
 * @param {*}
 * @return {*}
 */
void PlatformMonitor::PlatformMonitorThreadStart() {
  std::thread th = std::thread(&PlatformMonitor::PlatformMonitorThread, this);
  th.detach();
  // pthread_exit(NULL);
  // th.join();
}

/**
 * @description:平台监控线程执行函数
 * @param {*}
 * @return {*}
 */
void PlatformMonitor::PlatformMonitorThread() {
  /* CPU使用率监控 */
  GetCpuUseInfo(CpuUsageInfo1);
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    GetCpuUseInfo(CpuUsageInfo2);
    CalCpuUsage(CpuUsageInfo1, CpuUsageInfo2);

    /* 内存使用率监控 */
    GetMemUsage(MemUsageInfo);

    /* 磁盘使用率监控 */
    GetDiskUsage();

    /* CPU温度监控 */
    GetCPUTemperature();
  }
}

/**
 * @description:读/proc/stat文件，取第一行作为CPU当前状态数据的来源
 * @param {CpuUsageInfo_t} &coi
 * @return {*}
 */
void PlatformMonitor::GetCpuUseInfo(CpuUsageInfo_t &coi) {
  file_proc_stat_.open(proc_stat_path_.c_str(), ios::in);
  if (!file_proc_stat_.is_open()) {
    std::cerr << "open /proc/stat failed! " << std::endl;
  }
  std::string cpu_total_info;
  getline(file_proc_stat_, cpu_total_info);
  file_proc_stat_.close();
  if (cpu_total_info.empty()) {
    std::cerr << "read /proc/stat failed! " << std::endl;
  }
  std::stringstream ss(cpu_total_info);
  ss >> coi.name;
  ss >> coi.user;
  ss >> coi.nice;
  ss >> coi.system;
  ss >> coi.idle;
  ss >> coi.lowait;
  ss >> coi.irq;
  ss >> coi.softirq;
  return;
}

/**
 * @description:计算时间间隔内CPU的使用率
 * @param {const CpuUsageInfo_t} &t1 时间间隔开始时CPU状态
 * @param {const CpuUsageInfo_t} &t2 时间间隔结束时CPU状态
 * @return {*}
 */
void PlatformMonitor::CalCpuUsage(const CpuUsageInfo_t &t1,
                                  const CpuUsageInfo_t &t2) {
  unsigned long total_cpu_time_t1;
  unsigned long total_cpu_time_t2;
  total_cpu_time_t1 = (unsigned long)(t1.user + t1.nice + t1.system + t1.idle +
                                      t1.lowait + t1.irq + t1.softirq);
  total_cpu_time_t2 = (unsigned long)(t2.user + t2.nice + t2.system + t2.idle +
                                      t2.lowait + t2.irq + t2.softirq);
  /* 本次计算周期内CPU总时间 */
  double total = total_cpu_time_t2 - total_cpu_time_t1;
  /* 本次计算周期内CPU空闲时间 */
  double idle = t2.idle - t1.idle;
  /* 本次计算周期内CPU使用率 */
  cpu_usage_ = 100 * (static_cast<double>(total - idle) / total);
  /* 本次计算周期内CPU空闲率 */
  double cpu_idle = 100 * (idle / total);
  std::cout << "cpu_usage_ =" << std::setprecision(3) << cpu_usage_
            << std::endl;
  /* 存储下次计算周期的开始值 */
  CpuUsageInfo1 = CpuUsageInfo2;
  return;
}

/**
 * @description:读/proc/meminfo文件，从中读取内存状态数据，计算得到内存使用率
 *                              计算时free+buffers+cache当做可用内存
 * @param {MEM_OCCUPY} *mem
 * @return {*}
 */
void PlatformMonitor::GetMemUsage(MemInfo_t &mem) {
  file_proc_meminfo_.open(proc_meminfo_path_.c_str(), ios::in);
  if (!file_proc_meminfo_.is_open()) {
    std::cerr << "open /proc/meminfo failed! " << std::endl;
    return;
  }
  std::string mem_total_tmp;
  std::string mem_free_tmp;
  std::string mem_avail_tmp;
  std::string buffers_tmp;
  std::string cached_tmp;
  std::string swapcached_tmp;
  getline(file_proc_meminfo_, mem_total_tmp);
  getline(file_proc_meminfo_, mem_free_tmp);
  getline(file_proc_meminfo_, mem_avail_tmp);
  getline(file_proc_meminfo_, buffers_tmp);
  getline(file_proc_meminfo_, cached_tmp);
  getline(file_proc_meminfo_, swapcached_tmp);
  file_proc_meminfo_.close();
  if (mem_total_tmp.empty()) {
    std::cerr << "read /proc/stat failed! " << std::endl;
  }
  std::string mem_total;
  std::string mem_free;
  std::string mem_avail;
  std::string buffers;
  std::string cached;
  std::string swapcached;
  std::stringstream ss(mem_total_tmp);
  ss >> mem_total;
  ss >> mem.MemTotal;
  std::stringstream ss1(mem_free_tmp);
  ss1 >> mem_free;
  ss1 >> mem.MemFree;
  std::stringstream ss2(buffers_tmp);
  ss2 >> buffers;
  ss2 >> mem.Buffers;
  std::stringstream ss3(cached_tmp);
  ss3 >> cached;
  ss3 >> mem.Cached;
  std::stringstream ss4(swapcached_tmp);
  ss4 >> swapcached;
  ss4 >> mem.SwapCached;
  /* 内存使用率计算 */
  unsigned long memfree_for_program_ =
      mem.MemTotal - mem.MemFree - mem.Buffers - mem.Cached;
  mem_usage_ = 100 * (static_cast<double>(memfree_for_program_) / mem.MemTotal);
  std::cout << "mem_usage_ =" << std::setprecision(3) << mem_usage_
            << std::endl;
}

/**
 * @description:获得指定磁盘当前的使用率
 * @param {*}
 * @return {*}
 */
void PlatformMonitor::GetDiskUsage() {
  double diskrate = 0.0;
  struct statfs sf;
  if (0 == statfs(disk_dir_.c_str(), &sf)) {
    free_space_ = (((long long)sf.f_bsize * (long long)sf.f_bfree) / 1024);
    total_space_ = (((long long)sf.f_bsize * (long long)sf.f_blocks) / 1024);

    disk_usage_ =
        100 * (static_cast<double>(total_space_ - free_space_) / total_space_);
    std::cout << "disk_usage_  = " << disk_usage_ << std::endl;
  } else {
    std::cout << "read disk usage information error:" << std::endl;
  }
}

/**
 * @description: 获得CPU当前温度
 *               前提：需要先执行 sudo apt-get install lm-sensors 进行安装
 * @param {*}
 * @return {*}
 */
void PlatformMonitor::GetCPUTemperature() {
  FILE *fp;
  char line[100];
  std::stringstream ss_temperature_line;
  /* 执行linux指令得到cpu温度信息 */
  fp = popen(command_.c_str(), "r");
  if (fp == NULL) {
    std::cout << "commond exec failed!" << std::endl;
    return;
  }
  /* 得到温度所在行 */
  while (fgets(line, sizeof(line) - 1, fp) != NULL) {
    std::stringstream ss;
    for (int i = 0; i < 7; ++i) {
      ss << line[i];
    }
    if (ss.str() == "Package") {
      ss_temperature_line << line;
    }
    
  }
  std::string tmp1;
  std::string tmp2;
  std::string tmp3;
  ss_temperature_line >> tmp1;
  ss_temperature_line >> tmp2;
  ss_temperature_line >> tmp3;
  ss_temperature_line >> cpu_temperature_;
  std::cout << "cpu_temperature = " << cpu_temperature_ << std::endl;
}