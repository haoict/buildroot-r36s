################################################################################
#
# simple-terminal package
#
################################################################################

SIMPLE_TERMINAL_VERSION = 1.0
SIMPLE_TERMINAL_SITE = package/simple-terminal
SIMPLE_TERMINAL_SITE_METHOD = local# Other methods like git,wget,scp,file etc. are also available.

define SIMPLE_TERMINAL_BUILD_CMDS
    $(MAKE) UNION_PLATFORM=r36s-sdl12compat CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define SIMPLE_TERMINAL_INSTALL_TARGET_CMDS
    mkdir -p $(TARGET_DIR)/usr/local/bin
    $(INSTALL) -D -m 0755 $(@D)/build/SimpleTerminal  $(TARGET_DIR)/usr/local/bin
endef

$(eval $(generic-package))