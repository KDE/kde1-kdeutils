
INCLUDES = $(all_includes)
LDFLAGS  = $(all_libraries) $(KDE_RPATH)
LDADD =  -lkfile -lkfm       -lm $(LIB_KDEUI)

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


BUILTSOURCES =	klpq.moc ConfAutoUpdate.moc krowtable.moc \
kheader.moc queueview.moc FirstStart.moc FirstStartData.moc \
SpoolerConfig.moc SpoolerConfigData.moc
DISTCLEANFILES = $(BUILT_SOURCES)

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(klpq_SOURCES) && mv messages.po ../po/klpq.pot 

####### Meta objects 

klpq.cpp: klpq.moc
klpq.moc: klpq.h
	$(MOC) klpq.h -o klpq.moc

ConfAutoUpdate.cpp: ConfAutoUpdate.moc
ConfAutoUpdate.moc: ConfAutoUpdate.h
	$(MOC) ConfAutoUpdate.h -o ConfAutoUpdate.moc

krowtable.cpp: krowtable.moc
krowtable.moc: krowtable.h
	$(MOC) krowtable.h -o krowtable.moc

kheader.cpp: kheader.moc
kheader.moc: kheader.h
	$(MOC) kheader.h -o kheader.moc

queueview.cpp: queueview.moc
queueview.moc: queueview.h
	$(MOC) queueview.h -o queueview.moc

FirstStart.cpp: FirstStart.moc
FirstStart.moc: FirstStart.h
	$(MOC) FirstStart.h -o FirstStart.moc

FirstStartData.cpp: FirstStartData.moc
FirstStartData.moc: FirstStartData.h
	$(MOC) FirstStartData.h -o FirstStartData.moc

SpoolerConfig.cpp: SpoolerConfig.moc
SpoolerConfig.moc: SpoolerConfig.h
	$(MOC) SpoolerConfig.h -o SpoolerConfig.moc

SpoolerConfigData.cpp: SpoolerConfigData.moc
SpoolerConfigData.moc: SpoolerConfigData.h
	$(MOC) SpoolerConfigData.h -o SpoolerConfigData.moc
