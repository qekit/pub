#!/bin/bash
###
 # @Author: lj
 # @Date: 2021-05-21 11:38:10
 # @LastEditTime: 2021-06-17 19:43:05
 # @LastEditors: Please set LastEditors
 # @Description:PC2无人矿卡系统感知自启动脚本  1.放置在任意目录下 2.$gnome_session_properties 3.add加载刚放置脚本即可
 # @FilePath: ~/pc2_autostart.sh
### 
#说明：这是PC2上感知启动脚本上电自动执行的脚本
#1.测试ROSMaster是否通了
#2.只有RosMaster通了，才开始执行感知自起脚本

##### 配置 begin ####
#脚本执行日志文件
LOG_FILE=/home/${USER}/autostart.log
#ROS环境变量路径
ROS_PACKAGES_PATH=/opt/ros/melodic/setup.bash
#无人矿卡感知系统启动脚本run_perception.sh存放路径
SCRIPT_PATH=/home/${USER}/catkin_ws/shell/

#测试ROS Master是否通而尝试的次数
MASTER_COUNT=100
#两次尝试间隔
MASTER_WAIT=10
#TMP_FILE=/home/${USER}/.tmp.swap
##### 配置 end ####

index=1
#rm -f $TMP_FILE
#touch $TMP_FILE

#下面是PC2为了能够连接PC1上的rosmaster必须载入的环境变量
#其实可以将～/.bashrc中pc1和pc2换成IP地址，这样就不用export两句了，但是我不想改了
source /etc/profile
source /opt/ros/melodic/setup.bash
source /home/neousys/.bashrc
export ROS_HOSTNAME=192.168.0.13
export ROS_MASTER_URI=http://192.168.0.11:11311

while [ $index -le $MASTER_COUNT ]
     do
           roscore_stat=$(rosnode list)
           error_code=${roscore_stat:0:1}
           echo "$error_code"
           flag="/"
           if [[ $error_code = $flag ]];then
                d1=`date +%Y%m%d-%H:%M:%S`
                 echo "$d1 roscore ok" >>$LOG_FILE
                 cd $SCRIPT_PATH
                 source /opt/ros/melodic/setup.bash
                 source /home/neousys/.bashrc
                ./run_perception.sh
                 let index=1000
      #           rm -f $TMP_FILE
                 exit
           else
                 d1=`date +%Y%m%d-%H:%M:%S`
                 echo "$d1 cannot connect roscore" >>$LOG_FILE
                 let index++
                sleep $MASTER_WAIT
           fi
done
echo "$d1 try to  connect roscore more than 20 times, exit" >>$LOG_FILE
exit
