
pics_DATA = OpenBook.xpm filedel.xpm
picsdir = $(kde_datadir)/kjots/pics

icon_DATA = kjots.xpm
icondir = $(kde_icondir)

install-data-local: 
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kjots.xpm $(kde_minidir)/kjots.xpm
	
uninstall-local:
	rm -f $(kde_minidir)/kjots.xpm
