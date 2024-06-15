#!/bin/sh
echo "Hello, World! /tmp/boot.txt" > /tmp/boot.txt

modprobe dwc2
#modprobe exfat
#modprobe gpio_keys

mount -t vfat /dev/mmcblk0p1 /boot

nohup /usr/bin/python3 /usr/local/bin/joypad_hotkey.py &
nohup /usr/bin/python3 /usr/local/bin/power_hotkey.py &

sleep infinity