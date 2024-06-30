#!/bin/sh
if [ -e /dev/mmcblk0p3 ]; then
    # /dev/mmcblk0p3 already created.
    if grep -qs '/dev/mmcblk0p3' /proc/mounts;
        then echo "/dev/mmcblk0p3 already created and mounted. Exiting...";
        exit 0;
    fi

    # Format it as exfat.
    # parted /dev/mmcblk0 unit MiB print >> /dev/tty1 2>&1
    mkfs.exfat -n EASYROM /dev/mmcblk0p3 >> /dev/tty1 2>&1
    if [ $? -ne 0 ]; then
        echo "mkfs.exfat /dev/mmcblk0p3 failed" >> /dev/tty1
        exit 1
    fi
    sleep 5

    # Mount exfat partition to /roms
    rm -rf /roms
    mkdir -p /roms && mount -t exfat -o umask=0000,iocharset=utf8,noatime /dev/mmcblk0p3 /roms
    echo "/dev/mmcblk0p3 /roms exfat umask=0000,iocharset=utf8,noatime 0 0" >> /etc/fstab
    mount | grep /roms

    # Popluating /roms
    tar -Jxvf /roms.tar.xz -C /roms --no-same-owner >> /dev/tty1 2>&1

    # Cleanup
    mv /firstboot.sh /root/.firstboot-done.sh
    rm /roms.tar.xz

    echo "Formatting /dev/mmcblk0p3 done." >> /dev/tty1
    sleep 3
else
    # Create exfat partition
    # parted /dev/mmcblk0 mkpart primary ext4 2232320s 100% >> /dev/tty1 2>&1
    echo -e "n\np\n3\n2232320\n\nw\n" | fdisk /dev/mmcblk0 >> /dev/tty1 2>&1
    sleep 3
    # if [ $? -ne 0 ]; then
    #     echo "create partition /dev/mmcblk0p3 failed" >> /dev/tty1
    #     exit 1
    # fi

    echo -e "t\n3\n7\nw\n" | fdisk /dev/mmcblk0 >> /dev/tty1 2>&1
    sleep 3
    # if [ $? -ne 0 ]; then
    #     echo "update /dev/mmcblk0p3 to exFat/NTFS failed" >> /dev/tty1
    #     exit 1
    # fi
    
    partprobe /dev/mmcblk0 >> /dev/tty1 2>&1

    echo "Creating /dev/mmcblk0p3 done. Rebooting..." >> /dev/tty1
    sleep 3

    systemctl reboot -f
fi



