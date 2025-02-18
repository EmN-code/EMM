# include ../Makefile.bench_common
RIOTBASE ?= $(CURDIR)/../..

# include $(CURDIR)/../../Makefile.tests_common
APPLICATION ?= sched

# terminate native when the test is complete
CFLAGS += -DNATIVE_AUTO_EXIT=1

BOARD ?= esp32-wroom-32

QUIET ?= 1
# DEVELHELP enabled by default for all tests, set 0 to disable
DEVELHELP ?= 1


USEMODULE += xtimer



include $(RIOTBASE)/Makefile.include
