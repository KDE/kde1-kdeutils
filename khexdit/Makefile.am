 
DOCSDIR = $(prefix)/doc/HTML/khexdit
APPSDIR = $(prefix)/apps/Utilities

INCLUDES= @all_includes@
MOC	= @MOC@

####### Files

bin_PROGRAMS = 	khexdit
khexdit_SOURCES	= hexwidget.cpp hexfile.cpp libintl.c translater.cpp
noinst_HEADERS = libintlP.h hexfile.h hexwidget.h translater.h
khexdit_LDADD 	= @all_libraries@ -lkfm -lkdeui -lkdecore -lqt -lX11 -lXext @LIBSOCKET@ 
BUILTSOURCES =	hexwidget.moc hexfile.moc

hexwidget.cpp: hexwidget.moc
hexfile.cpp: hexfile.moc

%.moc: %.h
	$(MOC) $< -o $@

install-data-local: 
	$(mkinstalldirs) $(DOCSDIR)
	$(INSTALL_DATA) khexdit.html $(DOCSDIR)
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) "Hex Editor.kdelnk" $(APPSDIR)

uninstall-local:
	rm -f $(DOCSDIR)/$(DOCS)
	rm -f $(APPSDIR)/"Hex Editor.kdelnk"


