#!/bin/sh
echo "Hello, World! /tmp/boot.txt" > /tmp/boot.txt

modprobe -r dwc2
modprobe dwc2

mount -t vfat /dev/mmcblk0p1 /boot
mkdir -p roms && mount -t exfat /dev/mmcblk0p3 /roms

# sound setup
amixer set "Playback Path" SPK_HP
amixer set Playback 75%

# cpu normal
# echo dmc_ondemand > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/governor
# echo interactive > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
# echo dmc_ondemand > /sys/devices/platform/dmc/devfreq/dmc/governor

# cpu powersave
echo "userspace" > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/governor
echo "400000000" > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/userspace/set_freq
echo "userspace" > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
echo "1008000" > /sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed
echo "userspace" > /sys/devices/platform/dmc/devfreq/dmc/governor
echo "528000000" >  /sys/devices/platform/dmc/devfreq/dmc/userspace/set_freq

# cpu maxperf
# echo "performance" > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/governor
# echo "performance" > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
# echo "performance" > /sys/devices/platform/dmc/devfreq/dmc/governor


killall python3
nohup /usr/bin/python3 /usr/local/bin/joypad_hotkey.py &

cd /usr/local/bin && LD_LIBRARY_PATH=/usr/local/lib /usr/local/bin/simple-launcher &

sleep infinity