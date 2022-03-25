#!/bin/bash

# 日期：2021-12-30
# 用处：矿卡上面删除指定日志的脚本
# 使用方法：执行脚本时传入参数使用日期就行，
# 举例：./aht_del_log.sh 2021_11_1 就可以删除所有2021_11_10到2021_11_19内容

pre="*"
fix="*.log"

echo "delete $pre$1$fix"
# rm -rf $pre$1$fix

cd /usr/local/workspace/ahs_truck2/log/cmd && rm -rf $pre$1$fix &&
cd /usr/local/workspace/ahs_truck2/log/loc && rm -rf $pre$1$fix &&
cd /usr/local/workspace/ahs_truck2/log/tts && rm -rf $pre$1$fix &&
cd /usr/local/workspace/ahs_truck2/recording/   && rm -rf $pre$1$fix &&
cd /usr/local/workspace/ahs_truck2/recording/   && rm -rf $pre$1$fix &&
cd /usr/local/workspace/ahs_truck2/recording/AHTcommand   && rm -rf $pre$1$fix &&
cd /usr/local/workspace/obd_log  && rm -rf $pre$1$fix &&
cd /usr/local/workspace/obd_log  && rm -rf $pre$1$fix &&
cd /usr/local/workspace/obd_log  && rm -rf $pre$1$fix &&