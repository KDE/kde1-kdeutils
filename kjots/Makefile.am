
INCLUDES = $(all_includes)
LDFLAGS   = $(all_libraries) $(KDE_RPATH)
LDADD =  -lkfile -lkfm -lkdeui -lkdecore -lqt -lXext -lX11 -lm $(LIBSOCKET)

bin_PROGRAMS = kjots

kjots_SOURCES = main.cpp\
	KJotsMain.cpp\
	SubjList.cpp\
	ReadListConf.cpp\
	cpopmenu.cpp\
	confdialog.cpp\
	$(BUILT_SOURCES)

BUILT_SOURCES = mKJotsMain.cpp mSubjList.cpp mcpopmenu.cpp mconfdialog.cpp
DISTCLEANFILES = $(BUILT_SOURCES)

SUBDIRS = icons doc

appsdir = $(kde_appsdir)/Utilities
apps_DATA = Kjots.kdelnk

messages:

####### Meta objects

mKJotsMain.cpp: KJotsMain.h
	$(MOC) KJotsMain.h -o mKJotsMain.cpp

mSubjList.cpp: SubjList.h
	$(MOC) SubjList.h -o mSubjList.cpp

mcpopmenu.cpp: cpopmenu.h
	$(MOC) cpopmenu.h -o mcpopmenu.cpp

mconfdialog.cpp: confdialog.h
	$(MOC) confdialog.h -o mconfdialog.cpp








