
#INCDIR = /usr/X11R6/lib/qt/include
CFLAGS = -O2 -fno-strength-reduce -Wall -W \
-I$(QTDIR)/include -I/usr/X11R6/include -I$(KDEDIR)/include

LIBCFLAGS = -fPIC
YACCCFLAGS = -Wno-unused -Wno-parentheses
LFLAGS = -L/usr/X11R6/lib -lXext -L$(QTDIR)/lib -lqt \
-L$(KDEDIR)/lib -lkdeui -lkdecore -lkfm -lkfile

CXX = g++
MOC = $(QTDIR)/bin/moc
SHELL =	/bin/sh

####### Files

SOURCES =	kwmainwindow.cpp toplevel.cpp kwview.cpp kwdoc.cpp search.cpp highlight.cpp
HEADERS =	kwmainwindow.h toplevel.h kwview.h kwdoc.h search.h highlight.h
TARGET	=	kwrite


OBJECTS =	$(SOURCES:.cpp=.o)
OBJMETA =	$(HEADERS:.h=.m.o)

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJECTS) $(OBJMETA)
	$(CXX) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS) -lm

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

%.m.cpp: %.h
	$(MOC) -o $@ $<

#$(MOC) -o $(<:.h=.m.cpp) $<

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $<


clean:
	rm -f $(OBJECTS) $(OBJMETA) $(HEADERS:.h=.m.cpp) *~
	rm -f $(TARGET)


# DO NOT DELETE THIS LINE -- make depend depends on it.
