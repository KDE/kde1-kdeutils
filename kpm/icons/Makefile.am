
kde_icon_DATA = kpm.xpm

install-data-local:
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) $(srcdir)/mini-kpm.xpm $(kde_minidir)/kpm.xpm
	$(mkinstalldirs) $(kde_minidir)/../large
	$(INSTALL_DATA) $(srcdir)/large-kpm.xpm $(kde_minidir)/../large/kpm.xpm

uninstall-local:
	rm -f $(kde_minidir)/kpm.xpm
	rm -f $(kde_minidir)/../large/kpm.xpm
