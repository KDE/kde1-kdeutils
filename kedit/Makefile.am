#
#                           KEdit Makefile.am
#
#  Copyright 1997 Bernd Johannes Wuebben, wuebben@math.cornell.edu
#
#

INCLUDES = $(all_includes)
LDFLAGS	= $(all_libraries) $(KDE_RPATH)
LDADD = -lkfm -lkdecore -lkdeui -lqt -lXext -lX11 $(LIBSOCKET)

bin_PROGRAMS = kedit

kedit_SOURCES =	kedit.cpp  keditcl1.cpp keditcl2.cpp print.cpp\
		filldlg.cpp mail.cpp about.cpp urldlg.cpp
noinst_HEADERS = kedit.h  keditcl.h  print.h\
		filldlg.h  version.h mail.h about.h urldlg.h
BUILTSOURCES =	kedit.moc  keditcl.moc  print.moc\
		filldlg.moc mail.moc about.moc urldlg.moc
DISTCLEANFILES = $(BUILTSOURCES)

SUBDIRS = docs pixmaps

apps_DATA = KEdit.kdelnk
appsdir = $(kde_appsdir)/Applications

messages:
	@XGETTEXT@ -C -ktranslate -x ../../kdelibs/po/kde.pot $(kedit_SOURCES) && mv messages.po ../po/kedit.pot

kedit.cpp:kedit.moc
kedit.moc: kedit.h
	$(MOC) kedit.h -o kedit.moc

keditcl1.cpp:keditcl.moc
keditcl.moc: keditcl.h
	$(MOC) keditcl.h -o keditcl.moc

print.cpp:print.moc
print.moc: print.h
	$(MOC) print.h -o print.moc

filldlg.cpp:filldlg.moc
filldlg.moc: filldlg.h
	$(MOC) filldlg.h -o filldlg.moc

mail.cpp:mail.moc
mail.moc: mail.h
	$(MOC) mail.h -o mail.moc

about.cpp:about.moc
about.moc: about.h
	$(MOC) about.h -o about.moc

urldlg.cpp:urldlg.moc
urldlg.moc: urldlg.h
	$(MOC) urldlg.h -o urldlg.moc


