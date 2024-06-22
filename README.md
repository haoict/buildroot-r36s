# Buildroot for R36S
Buildroot is a simple, efficient and easy-to-use tool to generate embedded
Linux systems through cross-compilation.

The documentation can be found in docs/manual. You can generate a text
document with 'make manual-text' and read output/docs/manual/manual.text.
Online documentation can be found at http://buildroot.org/docs.html

## Build

To build and use the buildroot stuff, do the following:

1) run 'make r36s_defconfig'
2) run 'make menuconfig'
3) select the target architecture and the packages you wish to compile
4) run 'make -j$(nproc)'
5) wait while it compiles
6) find the kernel, bootloader, root filesystem, etc. in output/images

You do not need to be root to build or run buildroot.  Have fun!

Buildroot comes with a basic configuration for a number of boards. Run
'make list-defconfigs' to view the list of provided configurations.

Please feed suggestions, bug reports, insults, and bribes back to the
buildroot mailing list: buildroot@buildroot.org
You can also find us on #buildroot on OFTC IRC.

If you would like to contribute patches, please read
https://buildroot.org/manual.html#submitting-patches

## Install

### Make flashable img file

First download the original ArkOS img file: https://github.com/AeolusUX/ArkOS-R3XS

More info about u-boot:  
  - https://wiki.odroid.com/odroid_go_advance/build_uboot
  - https://github.com/christianhaitian/RG351V-u-boot

Ref about parted command: https://raspberrypi.stackexchange.com/questions/78466/how-to-make-an-image-file-from-scratch/78467#78467

| Partition | start (sector) | start (bytes) | size (sector) | size (bytes) |   requirement  |
|:---------:|:--------------:|:-------------:|:-------------:|:------------:|:--------------:|
| mbr       | 0              | 0             | 1             | 512          | -              |
| idbloader | 1              | 512           | 8191          | 4M - 512     | idbloader.img  |
| uboot     | 16384          | 8M            | 8192          | 4M           | uboot.mg       |
| trust     | 24576          | 12M           | 8192          | 4M           | trust.img      |
| BOOT      | 32768          | 16M           | 229376        | 112M         | /dev/mmcblk0p1 |
| rootfs    | 262144         | 128M          | -             | -            | /dev/mmcblk0p2 |


```bash
cd board/r36s
sudo ./post-build.sh
```
### Update rootfs only

```bash
sudo mount -t ext4 /dev/sdd2 /mnt/rootfs
sudo rm -rf /mnt/rootfs/*
sudo tar -xvf ~/projects/buildroot-r36s/output/images/rootfs.tar -C /mnt/rootfs && sync 
sudo umount /dev/sdd4
sudo eject /dev/sdd
```

## Clean target build without rebuild all binaries and libraries
https://stackoverflow.com/questions/47320800/how-to-clean-only-target-in-buildroot
```bash
rm -rf output/target
find output/ -name ".stamp_target_installed" -delete
rm -f output/build/host-gcc-final-*/.stamp_host_installed
```

## Appendix
### Run Docker
```bash
wget https://download.docker.com/linux/static/stable/aarch64/docker-26.1.4.tgz
tar -xzvf docker-26.1.4.tgz
cp docker/* /usr/bin/
dockerd &
docker run -p 8080:80 -d --name hello --rm nginxdemos/hello
docker ps -a
curl localhost:8080
```