#!/bin/bash
###BEGIN INIT INFO
#Providers:svnd.sh
#Required-start:$local_fs $remote_fs $network $syslog
#Required-stop:$local_fs $remote_fs $network $syslog
#Default-Start:2 3 4 5
#Default-Stop:0 1 6
#Short-Description:starts the svnd.sh daemon
#Description:starts svnd.sh using start-stop-daemon
###END INIT INFO

LOG_FILE=/home/neousys/scripts/time_update_err.log

INDEX=1
while [ $INDEX -le 1000 ]
	do
		# ping -c 1 -W 10 192.168.32.200
		# ping -c 1 -W 10 169.169.2.200
		ping -c 1 -W ntp1.aliyun.com
		if [[ $? != 0 ]];then
			date=`date +%Y/%m/%d-%H:%M:%S`
      echo "$date ERROR: cannot connect 192.168.32.200! Tried $INDEX times" >> $LOG_FILE
      let INDEX++
			sleep 10s
		else
			sudo ntpdate ntp1.aliyun.com && sudo hwclock -w
			exit
		fi
	done
