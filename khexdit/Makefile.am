 
DOCSDIR = $(prefix)/share/doc/HTML/khexdit
APPSDIR = $(prefix)/share/applnk/Utilities
ICONDIR = $(prefix)/share/icons/
MINIDIR = $(prefix)/share/icons/mini

INCLUDES= @all_includes@
MOC	= @MOC@

####### Files

bin_PROGRAMS = 	khexdit
khexdit_SOURCES	= hexwidget.cpp hexfile.cpp
noinst_HEADERS = hexfile.h hexwidget.h
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
	$(INSTALL_DATA) Khexdit.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(ICONDIR)
	$(INSTALL_DATA) khexdit.xpm $(ICONDIR)
	$(mkinstalldirs) $(MINIDIR)
	$(INSTALL_DATA) mini-khexdit.xpm $(MINIDIR)

uninstall-local:
	rm -f $(DOCSDIR)/$(DOCS)
	rm -f $(APPSDIR)/Khexdit.kdelnk
	rm -f $(ICONDIR)/khexdit.xpm
	rm -f $(MINIICONDIR)/mini-khexdit.xpm

