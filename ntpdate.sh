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

LOG_FILE=/home/neousys/scripts/ntpdate_error.log
# NTP_SERVER=192.168.32.200	#ABB
# NTP_SERVER=169.169.2.200	#5G
NTP_SERVER=ntp1.aliyun.com	#ali

INDEX=1
while [ $INDEX -le 1000 ]
	do
		ping -c 1 -W 10 $NTP_SERVER
		if [ $? -eq 0 ]; then
			echo "xcmg12345678" | sudo -S ntpdate -s $NTP_SERVER && sudo hwclock -w
			exit
		else
			date=`date +%Y/%m/%d-%H:%M:%S`
      echo "$date ERROR: cannot connect $NTP_SERVER ! Tried $INDEX times" >> $LOG_FILE
      let INDEX++
			sleep 30
		fi
	done
