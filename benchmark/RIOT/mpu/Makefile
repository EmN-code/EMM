# include ../Makefile.bench_common
RIOTBASE ?= $(CURDIR)/../..

# include $(CURDIR)/../../Makefile.tests_common
APPLICATION ?= mpu

# terminate native when the test is complete
CFLAGS += -DNATIVE_AUTO_EXIT=1

BOARD ?= esp32-wroom-32

QUIET ?= 1
# DEVELHELP enabled by default for all tests, set 0 to disable
DEVELHELP ?= 1


USEMODULE += od
USEMODULE += auto_init_gnrc_netif
USEMODULE += fmt

FEATURES_REQUIRED += periph_i2c
USEMODULE += ztimer
USEMODULE += ztimer_msec


CFLAGS += -DTEST_SUITES
CFLAGS += -DHAS_SOCK_DNS_MOCK=1

include $(RIOTBASE)/Makefile.include
