#############################################################################
# Makefile for building test1
# Generated by tmake at 11:45, 2007/04/14
#     Project: test1
#    Template: app
#############################################################################

####### Compiler, tools and options

CC	=	arm-linux-gcc
CXX	=	arm-linux-g++
CFLAGS	=	-pipe -Wall -W -O2 -DNO_DEBUG
CXXFLAGS=	-pipe -DQWS -fno-exceptions -fno-rtti -Wall -W -O2 -DNO_DEBUG -DQT_THREAD_SUPPORT
INCPATH	=	-I$(QTDIR)/include
LINK	=	arm-linux-g++
LFLAGS	=	-lezxjpeg -lezxpm -lezxappbase -lipp-jp -lezxopenwindow -lipp-miscGen -lezxappbase -lqte-mt 
LIBS	=	$(SUBLIBS) -L$(QTDIR)/lib
MOC	=	$(QTDIR)/bin/moc
UIC	=	$(QTDIR)/bin/uic

TAR	=	tar -cf
GZIP	=	gzip -9f

####### Files

HEADERS =	screenlock.h screenlockcanvas.h screenlockengine.h dapplication.h
SOURCES =	screenlock.cpp screenlockcanvas.cpp screenlockengine.cpp dapplication.cpp
OBJECTS =	screenlock.o screenlockcanvas.o screenlockengine.o dapplication.o
INTERFACES =	
UICDECLS =	
UICIMPLS =	
SRCMOC	=	
OBJMOC	=	
DIST	=	
TARGET	=	screenlocker
INTERFACE_DECL_PATH = .

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules


all: $(TARGET)

$(TARGET): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)

moc: $(SRCMOC)

tmake:
	tmake screenlock.pro

dist:
	tar czvf screenlock.pkg busybox screenlock.lin screenlock screenlock.gif screenlock_sm.gif swap.bin screenlock.desktop README.txt
	zip screenlock.zip screenlock.pkg README.txt

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC) $(UICIMPLS) $(UICDECLS) $(TARGET)
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

screenlock.o: screenlock.cpp screenlock.h
screenlockcanvas.o: screenlockcanvas.cpp screenlockcanvas.h
screenlockengine.o: screenlockengine.cpp screenlockengine.h
dapplication.o: dapplication.cpp dapplication.h

