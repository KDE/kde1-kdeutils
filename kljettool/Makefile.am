

DOCSDIR = @kde_htmldir@/en/kljettool
APPSDIR = @kde_appsdir@/Utilities/
PICSDIR = @kde_datadir@/kljettool/pics/

LDFLAGS	=	$(all_libraries) $(KDE_RPATH)
LDADD   =       -lkdecore -lqt -lm -lXext -lX11 $(LIBSOCKET)
INCLUDES=	$(all_includes)

####### Files

bin_PROGRAMS = kljettool
noinst_HEADERS = Paper.h PaperData.h  Fonts.h FontsData.h Printer.h \
	PrinterData.h Misc.h MiscData.h Data.h main.h about.h

kljettool_SOURCES = main.cpp Paper.cpp PaperData.cpp Fonts.cpp \
	FontsData.cpp Printer.cpp PrinterData.cpp Misc.cpp MiscData.cpp \
	Data.cpp about.cpp

BUILTSOURCES =	Paper.moc PaperData.moc Fonts.moc FontsData.moc Printer.moc\
		PrinterData.moc	Misc.moc MiscData.moc Data.moc main.moc \
		about.moc

DISTCLEANFILES = $(BUILTSOURCES)

SUBDIRS = docs support 

pics_DATA = kljetlogo.xpm
picsdir = $(kde_icondir)

apps_DATA = KLJetTool.kdelnk
appsdir = $(kde_appsdir)/Utilities/

install-data-local:
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kljettool.xpm $(kde_minidir)/kljettool.xpm
	@echo ""
	@echo ""
	@echo "If you own a RedHat system or if you have trouble getting"
	@echo "KLJetTool to work on your system, you must read the README"
	@echo "file in the directory support."
	@echo ""
	@echo "thanks,"
	@echo "Bernd Wuebben"
	@echo "wuebben@kde.org"
	@echo ""
	
uninstall-local:
	rm -f $(kde_minidir)/kljettool.xpm

messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(kljettool_SOURCES) && mv messages.po ../po/kljettool.pot

####### Meta objects

Paper.cpp: Paper.moc
Paper.moc: Paper.h
	$(MOC) Paper.h -o Paper.moc

Paper.cpp: PaperData.moc
PaperData.moc: PaperData.h
	$(MOC) PaperData.h -o PaperData.moc

Fonts.cpp: Fonts.moc
Fonts.moc: Fonts.h
	$(MOC) Fonts.h -o Fonts.moc

Fonts.cpp: FontsData.moc
FontsData.moc: FontsData.h
	$(MOC) FontsData.h -o FontsData.moc

Printer.cpp: Printer.moc
Printer.moc: Printer.h
	$(MOC) Printer.h -o Printer.moc

Printer.cpp: PrinterData.moc
PrinterData.moc: PrinterData.h
	$(MOC) PrinterData.h -o PrinterData.moc

Misc.cpp: Misc.moc
Misc.moc: Misc.h
	$(MOC) Misc.h -o Misc.moc

Misc.cpp: MiscData.moc
MiscData.moc: MiscData.h
	$(MOC) MiscData.h -o MiscData.moc

Data.cpp: Data.moc
Data.moc: Data.h
	$(MOC) Data.h -o Data.moc

main.cpp: main.moc
main.moc: main.h
	$(MOC) main.h -o main.moc

about.cpp: about.moc
about.moc: about.h
	$(MOC) about.h -o about.moc
