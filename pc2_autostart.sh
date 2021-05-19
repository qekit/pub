#!/bin/bash
#说明：这是PC2上感知启动脚本上电自动执行的脚本
#1.测试ROSMaster是否通了
#2.只有RosMaster通了，才开始执行感知自起脚本

##### 配置 begin ####
#脚本执行日志文件
LOG_FILE=/home/neousys/autostart_err.log
#ROS环境变量路径
ROS_PACKAGES_PATH=/opt/ros/melodic/setup.bash
#无人矿卡感知系统启动脚本run_perception.sh存放路径
SCRIPT_PATH=/home/neousys/catkin_ws/shell/

#测试ROS Master是否通而尝试的次数
MASTER_COUNT=20
#两次尝试间隔
MASTER_WAIT=10
##### 配置 end ####

index=1
source /opt/ros/melodic/setup.bash

while [ $index -le $MASTER_COUNT ]
     do
          roscore_stat=$(rosnode list)
          error_code=${roscore_stat:0:1}
          flag="/"
          if [[ $error_code = $flag ]];then
               d1=`date +%Y%m%d-%H:%M:%S`
                echo "$d1 roscore ok" >>$LOG_FILE
                cd $SCRIPT_PATH
               ./run_perception.sh &
                let index=1000
                exit
          else 
                d1=`date +%Y%m%d-%H:%M:%S`
                echo "$d1 roscore err" >>$LOG_FILE
                let index++
               sleep $MASTER_WAIT
          fi
done
