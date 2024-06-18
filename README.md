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

WIP


## Clean target build without rebuild all binaries and libraries
https://stackoverflow.com/questions/47320800/how-to-clean-only-target-in-buildroot
rm -rf output/target
find output/ -name ".stamp_target_installed" -delete
rm -f output/build/host-gcc-final-*/.stamp_host_installed

## Docker
```bash
wget https://download.docker.com/linux/static/stable/aarch64/docker-26.1.4.tgz
tar -xzvf docker-26.1.4.tgz
cp docker/* /usr/bin/
dockerd &
docker run -p 8080:80 -d --name hello --rm nginxdemos/hello
docker ps -a
curl localhost:8080
```

## Mounts
mkdir -p /roms && mount -t exfat /dev/mmcblk0p3 /roms
mkdir -p /mnt/rootfs && mount -t ext4 /dev/mmcblk0p2 /mnt/rootfs
