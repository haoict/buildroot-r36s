#!/bin/bash

cd board/r36s/roms_tar_xz
tar -Jcvf ../rootfs_overlay_systemd/roms.tar.xz .
cd ../../..