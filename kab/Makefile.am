# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

SUBDIRS = doc

INCLUDES= @all_includes@ 
# $(MS_NANA_DEFINES)
# -fexternal-templates 

APPSDIR = $(kde_appsdir)/Utilities
bin_PROGRAMS = 	kab kabapi_test
THE_LIBS= -lkfile -lkfm -lkdeui -lkdecore -lqt -lX11 \
		-lXext @LIBSOCKET@ -ljpeg -lkab

kab_SOURCES	= main.cc \
		AddressBookAbout.cpp \
		AddressBookAboutData.cpp \
		toplevelwidget.cc
kab_LDFLAGS = @all_libraries@
kab_LDADD   = 	$(THE_LIBS)

kabapi_test_SOURCES= kabapi_test.cc
kabapi_test_LDFLAGS = @all_libraries@
kabapi_test_LDADD= $(THE_LIBS)


# I have added all additional files that need to be in the archive here.
# Hopefully this does not harm.
noinst_HEADERS = 
BUILTSOURCES = 	toplevelwidget.moc \
		AddressBookAboutData.moc \
		AddressBookAbout.moc 

# CLEANFILES = $(BUILTSOURCES)
# DISTCLEANFILES = $(BUILTSOURCES)

messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(kab_SOURCES) && mv messages.po ../po/kab.pot

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) kab.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) kab.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kab.xpm $(kde_minidir)/kab.xpm
	$(mkinstalldirs) $(kde_datadir)
	$(mkinstalldirs) $(kde_datadir)/kab/
	$(mkinstalldirs) $(kde_datadir)/kab/pics/
	$(INSTALL_DATA) background_1.jpg \
		$(kde_datadir)/kab/pics/background_1.jpg
	$(INSTALL_DATA) addressbook_logo.jpg \
		$(kde_datadir)/kab/pics/addressbook_logo.jpg
	

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(APPSDIR)/kab.kdelnk
	-rm -f $(kde_icondir)/kab.xpm
	-rm -f $(kde_minidir)/kab.xpm

%.moc:%.h
	$(MOC) $< -o $@

toplevelwidget.cc:toplevelwidget.moc
toplevelwidget.moc:toplevelwidget.h

AddressBookAbout.cpp:AddressBookAbout.moc AddressBookAboutData.moc
AddressBookAbout.moc:AddressBookAbout.h
AddressBookAboutData.moc:AddressBookAboutData.h
