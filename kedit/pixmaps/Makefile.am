
icon_DATA = kedit.xpm
icondir = $(kde_icondir)

toolbar_DATA = filenew2.xpm send.xpm
toolbardir = $(kde_datadir)/kedit/toolbar

pics_DATA = keditlogo.xpm
picsdir = $(kde_datadir)/kedit/pics

EXTRA_DIST = $(icon_DATA) $(toolbar_DATA) $(pics_DATA) mini-kedit.xpm

install-data-local: 
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kedit.xpm  $(kde_minidir)/kedit.xpm


uninstall-local:
	rm -f $(kde_minidir)/kedit.xpm
