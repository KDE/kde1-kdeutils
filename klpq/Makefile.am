
INCLUDES = $(all_includes)
LDFLAGS  = $(all_libraries) $(KDE_RPATH)
LDADD =          -lm $(LIB_KFILE)

SUBDIRS = doc

bin_PROGRAMS = 	klpq

klpq_SOURCES =  main.cpp\
		klpq.cpp\
		ConfAutoUpdate.cpp\
		ConfAutoUpdateData.cpp\
		krowtable.cpp\
		kheader.cpp\
		queueview.cpp\
		klpqspooler.cpp\
		FirstStart.cpp\
		FirstStartData.cpp\
		SpoolerConfig.cpp\
		SpoolerConfigData.cpp

appsdir = $(kde_appsdir)/Utilities
apps_DATA = KLpq.kdelnk


METASOURCES =	klpq.moc ConfAutoUpdate.moc krowtable.moc \
kheader.moc queueview.moc FirstStart.moc FirstStartData.moc \
SpoolerConfig.moc SpoolerConfigData.moc

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(klpq_SOURCES) && mv messages.po ../po/klpq.pot 

kde_icon_DATA = klpq.xpm

install-data-local:
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-klpq.xpm $(kde_minidir)/klpq.xpm

uninstall-local:
	rm -f $(kde_minidir)/klpq.xpm

