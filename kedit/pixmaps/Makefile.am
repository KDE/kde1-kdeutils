
icon_DATA = kedit.xpm
icondir = $(kde_icondir)

toolbar_DATA = filenew2.xpm send.xpm
toolbardir = $(kde_datadir)/kedit/toolbar

pics_DATA = keditlogo.xpm
picsdir = $(kde_datadir)/kedit/pics

install-data-local: 
	$(INSTALL_DIR) $(kde_minidir)
	$(INSTALL_DATA) pixmaps/mini-kedit.xpm  $(kde_minidir)/kedit.xpm


uninstall-local:
	rm -f $(MINIDIR)/kedit.xpm
