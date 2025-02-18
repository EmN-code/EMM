# include ../Makefile.bench_common
RIOTBASE ?= $(CURDIR)/../..

# include $(CURDIR)/../../Makefile.tests_common
APPLICATION ?= flash

# terminate native when the test is complete
CFLAGS += -DNATIVE_AUTO_EXIT=1

BOARD ?= esp32-wroom-32

QUIET ?= 1
# DEVELHELP enabled by default for all tests, set 0 to disable
DEVELHELP ?= 1


USEMODULE += xtimer

USEMODULE += od
USEMODULE += auto_init_gnrc_netif
USEMODULE += fmt



CFLAGS += -DTEST_SUITES
CFLAGS += -DHAS_SOCK_DNS_MOCK=1

include $(RIOTBASE)/Makefile.include
