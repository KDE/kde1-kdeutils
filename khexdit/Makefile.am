 
APPSDIR = $(kde_appsdir)/Utilities
INCLUDES= $(all_includes)

####### Files

SUBDIRS = doc
bin_PROGRAMS = 	khexdit
khexdit_SOURCES	= hexwidget.cpp hexfile.cpp
noinst_HEADERS = hexfile.h hexwidget.h
khexdit_LDADD = $(all_libraries) -lkfm -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET)
khexdit_LDFLAGS = $(LDFLAGS) $(KDE_RPATH)

BUILTSOURCES = hexwidget.moc hexfile.moc
DISTCLEANFILES = hexwidget.moc hexfile.moc

hexwidget.cpp:hexwidget.moc
hexwidget.moc: hexwidget.h
	$(MOC) hexwidget.h -o hexwidget.moc

hexfile.cpp:hexfile.moc
hexfile.moc: hexfile.h
	$(MOC) hexfile.h -o hexfile.moc

install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) khexdit.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) khexdit.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-khexdit.xpm $(kde_minidir)/khexdit.xpm

uninstall-local:
	rm -f $(APPSDIR)/khexdit.kdelnk
	rm -f $(kde_icondir)/khexdit.xpm
	rm -f $(kde_minidir)/khexdit.xpm

