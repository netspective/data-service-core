# Makefile 

TOPDIR := $(shell pwd)

#ifeq   ($(PREFIX),"")
ifeq ($(strip $(PREFIX)),)

	TARGET_PATH=$(TOPDIR)
else
	TARGET_PATH=$(PREFIX)
endif

CPPFLAGS= -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys -I$(TOPDIR)/src/opensplice -I$(TOPDIR)/src -I$(TOPDIR)/src/logger 
LDFLAGS= -L$(OSPL_HOME)/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib -L$(TOPDIR)/src/opensplice -L$(TOPDIR)/src -L$(TOPDIR)/src/logger
LIBS= -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  -lpthread
BOOST_LIBS= -lboost_thread -lboost_program_options -lboost_system

# The target programs to build
TARGETS= \
	SimpleDDS


# The target programs to build
INSTALL_TARGETS= \
	SimpleDDS_Install


# Build all targets
all: $(TARGETS)

# Build all targets
install: $(INSTALL_TARGETS)

# Clean all files, including C++ generated from IDL
clean:
	rm -f $(TOPDIR)/src/*.o
	rm -f $(TOPDIR)/src/OpenSplice/*.o
	rm -rf $(TOPDIR)/include
	rm -rf $(TOPDIR)/lib
	rm -f $(TOPDIR)/*.a
	rm -f /lib/libsimpledds.a
	rm -f /usr/include/DataService.h
	rm -f /usr/include/OpenSpliceDataService.h


SimpleDDS:
	cd $(TOPDIR)/src; \
	/usr/bin/g++ $(CPPFLAGS) -c DataService.cpp -o DataService.o ; \
	cd $(TOPDIR)/src/OpenSplice; \
	/usr/bin/g++ $(CPPFLAGS) -c OpenSpliceDataService.cpp -o OpenSpliceDataService.o; \
	ar cr $(TOPDIR)/libsimpledds.a $(TOPDIR)/src/DataService.o $(TOPDIR)/src/OpenSplice/OpenSpliceDataService.o /lib/liblogger.a; \
	cd $(TOPDIR); \
        ranlib libsimpledds.a;

SimpleDDS_Install:
	mkdir -p $(TARGET_PATH)/lib; \
	mkdir -p $(TARGET_PATH)/include; \
	cp $(TOPDIR)/src/DataService.h $(TARGET_PATH)/include/DataService.h; \
	cp $(TOPDIR)/src/DataService.h /usr/include/DataService.h; \
        cp $(TOPDIR)/src/OpenSplice/OpenSpliceDataService.h $(TARGET_PATH)/include/OpenSpliceDataService.h; \
        cp $(TOPDIR)/src/OpenSplice/OpenSpliceDataService.h /usr/include/OpenSpliceDataService.h; \
        cp libsimpledds.a $(TARGET_PATH)/lib ;\
        cp libsimpledds.a /lib

