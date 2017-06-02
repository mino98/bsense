# D-ITG MinGW Main Makefile

###################
# Dynamic Options #
###################

# Switch to IPv6 mode if specified on command line
ifdef ipv6	
	CXXOPT := $(CXXOPT) -DIPv6RECV
endif		

# Set debug level as specified on command line
ifdef debug
	CXXOPT := $(CXXOPT) -DDEBUG=$(debug)
else
	CXXOPT := -O2 $(CXXOPT)
endif

##################
# Static options # 
##################

# Locals
GWDIR    = /usr/i586-mingw32msvc/
CXXINCS  = -I"$(GWDIR)/include"
LIBS     = -L"$(GWDIR)/lib"
INCS     = -I"$(GWDIR)/include" 
CFLAGS   = $(INCS)  

SUBDIRS = ITGSend ITGRecv ITGLog ITGDec libITG ITGManager

# Globals
export OS       = WIN32
export SUFFIX   = .exe
export CXXFLAGS = $(CXXOPT) -Wall -D$(OS) -D_WIN32_WINNT=0x0501 -Wno-deprecated $(CXXINCS)
export LDFLAGS  = $(GWDIR)/lib/libws2_32.a 
export CXX      = i586-mingw32msvc-g++
export CC       = i586-mingw32msvc-gcc
export AR       = i586-mingw32msvc-ar
export RANLIB   = i586-mingw32msvc-ranlib

export RM = rm -rf
export CP = cp
export MV = mv

export BASEDIR = $(shell pwd)
export BIN     = $(BASEDIR)/../bin/
export COMMON  = $(BASEDIR)/common/
export NRCLEAN = $(BASEDIR)/ITGSend/newran/

export THOBJS = common/thread.o
export OBJS   = common/ITG.o common/timestamp.o common/serial.o

#################
# Generic Rules #
#################

.PHONY: $(SUBDIRS)
all: $(SUBDIRS)
	@ echo '-----------------------------------------------------------------------------------'
	@ echo 'D-ITG executables created in "bin" folder'
ifndef ipv6
	@ echo 'To obtain IPv6 binaries add "ipv6=on" option to make command line'
else
	@ echo 'IPv6 mode enabled'
endif
ifndef debug
	@ echo 'To enable debug mode (verbosity level *) add "debug=*" option to make command line'
else
	@ echo 'Debug mode enabled (verbosity level = '$(debug)')'
endif
	@ echo 'NOTE: Changing previous options requires a "make clean" before'


ITGSend ITGRecv ITGLog ITGDec libITG: $(THOBJS) $(OBJS)
	@ (cd $@ && $(MAKE))

ITGManager: 
	@ (cd $@ && $(MAKE))

clean: 
	(cd "$(COMMON)" && $(RM) *.o) 
	(cd ITGSend && make clean) 
	(cd ITGRecv && make clean) 
	(cd ITGLog && make clean) 
	(cd ITGDec && make clean) 
	(cd libITG && make clean) 
	(cd ITGManager && make clean) 

