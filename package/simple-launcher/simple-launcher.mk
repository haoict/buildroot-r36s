################################################################################
#
# simple-launcher package
#
################################################################################

SIMPLE_LAUNCHER_VERSION = 1.0
SIMPLE_LAUNCHER_SITE = package/simple-launcher
SIMPLE_LAUNCHER_SITE_METHOD = local# Other methods like git,wget,scp,file etc. are also available.

define SIMPLE_LAUNCHER_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define SIMPLE_LAUNCHER_INSTALL_TARGET_CMDS
    mkdir -p $(TARGET_DIR)/usr/local/bin
    $(INSTALL) -D -m 0755 $(@D)/simple-launcher  $(TARGET_DIR)/usr/local/bin
    $(INSTALL) -D -m 0755 $(@D)/simple-launcher-commands.txt  $(TARGET_DIR)/usr/local/bin
    $(INSTALL) -D -m 0755 $(@D)/simple-launcher.ttf  $(TARGET_DIR)/usr/local/bin
endef

$(eval $(generic-package))