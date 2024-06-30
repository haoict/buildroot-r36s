#!/bin/sh
if [ -e /dev/mmcblk0p3 ]; then echo "/dev/mmcblk0p3 already created"; exit 0; fi >> /dev/tty1 2>&1

# Create exfat partition
# parted /dev/mmcblk0 mkpart primary ext4 2232320s 100% >> /dev/tty1 2>&1
echo -e "n\np\n3\n2232320\n\nw\n" | fdisk /dev/mmcblk0 >> /dev/tty1 2>&1
echo -e "t\n3\n7\nw\n" | fdisk /dev/mmcblk0 >> /dev/tty1 2>&1
partprobe /dev/mmcblk0 >> /dev/tty1 2>&1
# parted /dev/mmcblk0 unit MiB print >> /dev/tty1 2>&1
mkfs.exfat -n EASYROM /dev/mmcblk0p3 >> /dev/tty1 2>&1

# Mount exfat partition to /roms
mkdir -p /roms && mount -t exfat -o umask=0000,iocharset=utf8,noatime /dev/mmcblk0p3 /roms
echo "/dev/mmcblk0p3 /roms exfat umask=0000,iocharset=utf8,noatime 0 0" >> /etc/fstab
mount | grep /roms

# Popluating /roms
tar -Jxvf /roms.tar.xz -C /roms >> /dev/tty1 2>&1

# Cleanup
mv /firstboot.sh /root/.firstboot-done.sh
rm /roms.tar.xz
