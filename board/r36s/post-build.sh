#!/bin/bash
# Check if user is root
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root." 
   exit 1
fi

cd ../../output/images

rm -f buildroot-r36s.img

# Create an empty img file
truncate -s 1090M buildroot-r36s.img

# Make the disk MBR type (msdos)
parted buildroot-r36s.img mktable msdos

# Copy u-boot data (skip the first sector because it is MBR data)
dd if=../../board/r36s/uboot.img of=buildroot-r36s.img bs=512 skip=1 seek=1 count=32767 conv=notrunc

# Making BOOT partitions (size 135168 sector - 32768 sector = 102400 sectors * 512 = 50MiB)
parted buildroot-r36s.img mkpart primary fat32 32768s 135167s

# Making rootfs partitions (size 1Gi)
parted buildroot-r36s.img mkpart primary ext4 135168s 2232319s

# Verify
parted buildroot-r36s.img unit MiB print

# Format partitions and mount
DEV_LOOP=$(losetup --show --find --partscan buildroot-r36s.img)
ls -la "${DEV_LOOP}"*
mkfs.fat -F32 -n BOOT ${DEV_LOOP}p1
mkfs.ext4 -L rootfs ${DEV_LOOP}p2

mkdir -p /mnt/BOOT && mount -t vfat ${DEV_LOOP}p1 /mnt/BOOT
mkdir -p /mnt/rootfs && mount -t ext4 ${DEV_LOOP}p2 /mnt/rootfs

# Copy the content of orignal ArkOS BOOT (Kernel, uInitrd, DTB, ...) to /mnt/BOOT
tar -xJf ../../board/r36s/BOOT.tar.xz -C /mnt/BOOT --no-same-owner
# Extract buildroot (output/images/rootfs.tar) to /mnt/rootfs
tar -xf rootfs.tar -C /mnt/rootfs --no-same-owner
sync

# Unmount and Detach the img
umount /mnt/BOOT
umount /mnt/rootfs
losetup --detach-all

cd ../../board/r36s

echo "Done."
