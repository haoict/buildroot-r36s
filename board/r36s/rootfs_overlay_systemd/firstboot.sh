#!/bin/sh
if [ -e /dev/mmcblk0p3 ]; then echo "/dev/mmcblk0p3 already created"; exit 0; fi >> /dev/tty1 2>&1
parted /dev/mmcblk0 mkpart primary ext4 2232320s 100% >> /dev/tty1 2>&1
parted /dev/mmcblk0 unit MiB print >> /dev/tty1 2>&1
mkfs.exfat -n EASYROM /dev/mmcblk0p3 >> /dev/tty1 2>&1
mkdir -p /roms && mount -t exfat /dev/mmcblk0p3 /roms
echo "/dev/mmcblk0p3 /roms exfat defaults 0 0" >> /etc/fstab
tar -xvf /roms.tar -C / >> /dev/tty1 2>&1
mv /firstboot.sh /root/.firstboot-done.sh
rm /roms.tar
