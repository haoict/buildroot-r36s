include makefiles/config.mk

ifeq ($(UNION_PLATFORM),rg35xxplus)
include makefiles/config-rg35xxplus.mk
else ifeq ($(UNION_PLATFORM),r36s)
include makefiles/config-r36s.mk
else ifeq ($(UNION_PLATFORM),trimuisp)
include makefiles/config-trimuisp.mk
endif


.PHONY: clean
.PHONY: simple-launcher

simple-launcher: simple-launcher.c
	$(CC) -o '$@' '$<' ${CFLAGS} ${LDFLAGS}

clean:
	-rm simple-launcher