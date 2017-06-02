#   D-ITG Main Makefile

# BSense options:
static=yes 
noipv6=on 
onoff=on

###################
# Dynamic Options #
###################

# Enable SCTP support if specified on command line (requires kernel version >= 2.5.15)
ifdef sctp
	CXXOPT += -DSCTP
	LDOPT  += -lsctp
endif

# Enable DCCP support if specified on command line (requires kernel version >= 2.6.14-rc1)
ifdef dccp
	CXXOPT += -DDCCP
endif

# Enable ARM support
ifdef arm
	CXXOPT += -DARM
endif

# Set debug level as specified on command line (debug level may be increased to have more information)
ifdef debug
	CXXOPT += -DDEBUG=$(debug) -ggdb
endif

# Enable OnOff traffic types if specified on command line.
ifdef onoff
	CXXOPT += -DONOFF
endif

# Disable IPv6 support
ifdef noipv6
	CXXOPT += -DNOIPV6
endif


##################
# Static options # 
##################

# OS options
OS := $(shell uname)
ifeq "$(OS)" "Linux"
    OS := -DLINUX_OS

    # Static binaries
    ifdef static
	CXXOPT += -static
    endif
endif
ifeq "$(OS)" "FreeBSD"
    OS := -DLINUX_OS -DBSD
endif
ifeq "$(OS)" "Darwin"
    OS := -DLINUX_OS -DBSD -DOSX

    # Warning! At the moment D-ITG has no support for IPv6 on Apple OSX:
    CXXOPT += -DNOIPV6
endif

export OS
export RM = rm -f
export CP = cp
export MV = mv
export SUFFIX =
export CXXFLAGS = $(CXXOPT) $(OS) -Wall -Wno-deprecated -fPIC
export LDFLAGS = -lpthread $(LDOPT)
export BASEDIR = $(shell pwd)
export BIN = $(BASEDIR)/../bin/
export COMMON = $(BASEDIR)/common/
export NRCLEAN = $(BASEDIR)/ITGSend/newran/
export EXEC_DIR = /usr/local/bin

ifndef CXX
	CXX = g++
endif
ifndef RANLIB
	export RANLIB = ranlib
endif

export THOBJS = common/thread.o
export OBJS = common/ITG.o common/timestamp.o common/serial.o
# NOTE: in this BSense version, we build only ITGSend, ITGRecv and ITGDec:
#SUBDIRS = ITGSend ITGRecv ITGLog ITGDec libITG ITGManager
SUBDIRS = ITGSend ITGRecv ITGDec

#################
# Generic Rules #
#################

.PHONY: $(SUBDIRS)
all: $(SUBDIRS)
	@ echo '--------------------------------------------------------------------------------------'
	@ echo ' Note: this is D-ITG BSense edition, currently a patched version of D-ITG-2.8.0-Beta'
	@ echo '--------------------------------------------------------------------------------------'
	@ echo 'D-ITG executables created in "bin" folder'

# NOTE: in this BSense version, we build only ITGSend, ITGRecv and ITGDec:
#ITGSend ITGRecv ITGLog ITGDec libITG: $(THOBJS) $(OBJS)
#	@ (cd $@ && $(MAKE))
#ITGManager:
#	@ (cd $@ && $(MAKE))

ITGSend ITGRecv ITGDec: $(THOBJS) $(OBJS)
	@ (cd $@ && $(MAKE))

# NOTE: in this BSense version, no 'install' needed
#install:
#	(cd ITGSend && $(MAKE) install)
#	(cd ITGRecv && $(MAKE) install)
#	(cd ITGLog && $(MAKE) install)
#	(cd ITGDec && $(MAKE) install)
#	(cd ITGManager && $(MAKE) install)

# NOTE: in this BSense version, no 'uninstall' needed
#uninstall:
#	(cd /usr/local/bin && rm ITGSend ITGRecv ITGLog ITGDec ITGManager)
	
# NOTE: in this BSense version, we build only ITGSend, ITGRecv and ITGDec:
clean:
	$(RM) "$(COMMON)"*.o
	(cd ITGSend && $(MAKE) clean)
	(cd ITGRecv && $(MAKE) clean)
	(cd ITGDec && $(MAKE) clean)
#	(cd ITGLog && $(MAKE) clean)
#	(cd libITG && $(MAKE) clean)
#	(cd ITGManager && $(MAKE) clean)

