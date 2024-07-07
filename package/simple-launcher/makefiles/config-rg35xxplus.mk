export LD_LIBRARY_PATH=/opt/rg35xxplus-toolchain/usr/lib

CROSS_COMPILE=/opt/rg35xxplus-toolchain/usr/bin/arm-buildroot-linux-gnueabihf-
SYSROOT = /opt/rg35xxplus-toolchain/usr/arm-buildroot-linux-gnueabihf/sysroot

CFLAGS += -I$(SYSROOT)/usr/include -DRG35XXP
LDFLAGS += -L${SYSROOT}/usr/lib -lpthread -Wl,-rpath-link,$(SYSROOT)/usr/lib
