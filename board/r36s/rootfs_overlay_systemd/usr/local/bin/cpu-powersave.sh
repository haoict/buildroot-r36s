#!/bin/sh
echo "userspace" > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/governor
echo "400000000" > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/userspace/set_freq
echo "userspace" > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
echo "1008000" > /sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed
echo "userspace" > /sys/devices/platform/dmc/devfreq/dmc/governor
echo "528000000" >  /sys/devices/platform/dmc/devfreq/dmc/userspace/set_freq