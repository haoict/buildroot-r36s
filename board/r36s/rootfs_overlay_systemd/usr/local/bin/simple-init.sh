#!/bin/sh

if [ -f /firstboot.sh ]; then
    /firstboot.sh
fi

echo 3 > /proc/sys/kernel/printk

# Disable console blanking
echo -ne "\033[9;0]" > /dev/tty1

# sound setup
amixer set "Playback Path" SPK_HP
amixer set Playback 75%

/usr/local/bin/cpu-powersave.sh

killall python3
nohup /usr/bin/python3 /usr/local/bin/simple-keymon.py &

cd /usr/local/bin && LD_LIBRARY_PATH=/usr/local/lib /usr/local/bin/simple-launcher &

sleep infinity