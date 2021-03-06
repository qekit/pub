#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-05-18 17:36:37
 # @LastEditTime: 2021-06-17 18:17:26
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /catkin_ws/git-lnk/lj/shell_script_/pc1_autostart.sh
### 
#说明：PC1上无人系统启动脚本上电自动执行脚本
#1.测试与机群的通信
#2.与机群可通信，执行启动脚本

##### 配置 begin ####
#脚本执行日志文件
LOG_FILE=/home/neousys/autostart_err.log
#机群地址
FLEET_IP=192.168.32.201
#无人矿卡系统启动run.sh存放路径
SCRIPT_PATH=/usr/local/workspace/ahs_truck_debug/
#测试通讯而尝试ping机群的次数
FLEET_COUNT=100
#每次ping机群的时候等待时长
FLEET_WAIT=10
##### 配置 end ####
export ROS_HOSTNAME=192.168.0.11
a=1
while [ $a -le $FLEET_COUNT ]
    do
        ping -c 1 -W $FLEET_WAIT $FLEET_IP
        if [ $? -eq 0 ]; then
            cd $SCRIPT_PATH
            sleep 120
            source /opt/ros/melodic/setup.bash
            source /home/neousys/.bashrc
            ./run.sh &
            let a=1000
        else
            d1=`date +%Y%m%d-%H:%M:%S`
            echo "$d1 ERROR: connect to fleet failed! Tried $a times">>$LOG_FILE
            let a++                     
        fi
    done
