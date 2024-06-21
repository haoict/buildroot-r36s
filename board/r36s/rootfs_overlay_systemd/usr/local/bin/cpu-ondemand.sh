#!/bin/sh
echo dmc_ondemand > /sys/devices/platform/ff400000.gpu/devfreq/ff400000.gpu/governor
echo interactive > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
echo dmc_ondemand > /sys/devices/platform/dmc/devfreq/dmc/governor