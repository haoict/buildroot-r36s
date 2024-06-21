#!/bin/sh
echo "performance" > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/governor
echo "performance" > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
echo "performance" > /sys/devices/platform/dmc/devfreq/dmc/governor