# Uncomment following line to switch to IPv6 mode using Dev-Cpp IDE
#CXXIPV6 = "-DIPv6RECV"

# Switch to IPv6 mode if specified on command line
ifdef ipv6	
	CXXIPV6 = "-DIPv6RECV"
endif		

# Uncomment following line to enable debug output using Dev-Cpp IDE (debug level may be increased to have more information)
#CXXDEBUG = -DDEBUG=1

# Set debug level as specified on command line
ifdef debug
	CXXDEBUG = -DDEBUG=$(debug)
endif

export DEVDIR=$(SystemDrive)/dev-cpp
export OS = WIN32
export RM = rm -rf
export CP = cmd /c copy
export MV = cmd /c move
export SUFFIX = .exe
export CXXFLAGS = -O2 $(CXXDEBUG) -Wall $(CXXIPV6) -DWIN32 -Wno-deprecated -I$(DEVDIR)/Include/G++ -I$(DEVDIR)/Include/c++ -I$(DEVDIR)/Include/c++/mingw32 -I$(DEVDIR)/Include/c++/backward -I$(DEVDIR)/Include -L$(DEVDIR)/Lib
export LDFLAGS = $(DEVDIR)/Lib/libws2_32.a 
export BASEDIR = $(shell cmd /c cd)
export BIN = $(BASEDIR)\..\bin\\
export COMMON = $(BASEDIR)\common\\
export NRCLEAN = $(BASEDIR)\ITGSend\newran\\
export EXEC_DIR = $(BASEDIR)\..\bin\\
export RANLIB = ranlib

CXX = g++
export THOBJS = common/thread.o
export OBJS = common/ITG.o common/timestamp.o common/serial.o
SUBDIRS = ITGSend ITGRecv ITGLog ITGDec libITG ITGManager

.PHONY: $(SUBDIRS)
all: $(SUBDIRS)
	@ echo '-----------------------------------------------------------------------------------'
	@ echo 'D-ITG executables created in "bin" folder'
ifndef CXXIPV6
	@ echo 'To obtain IPv6 binaries add "ipv6=on" option to make command line'
else
	@ echo 'IPv6 mode enabled'
endif
ifndef CXXDEBUG
	@ echo 'To enable debug mode (verbosity level *) add "debug=*" option to make command line'
else
	@ echo 'Debug mode enabled (verbosity level = '$(debug)')'
endif
	@ echo 'NOTE: Changing previous options requires a "make clean" before'

ITGSend ITGRecv ITGLog ITGDec libITG: $(THOBJS) $(OBJS)
	@ (cd $@ && $(MAKE))

ITGManager: 
	@ (cd $@ && $(MAKE))

install:
	(cd ITGSend && make install) 
	(cd ITGRecv && make install) 
	(cd ITGLog && make install) 
	(cd ITGDec && make install) 
	(cd ITGManager && make install) 

uninstall:
	(cd /usr/local/bin && rm ITGSend ITGRecv ITGLog ITGDec ITGManager)

clean: 
	(cd "$(COMMON)" && $(RM) *.o) 
	(cd ITGSend && make clean) 
	(cd ITGRecv && make clean) 
	(cd ITGLog && make clean) 
	(cd ITGDec && make clean) 
	(cd libITG && make clean) 
	(cd ITGManager && make clean) 

