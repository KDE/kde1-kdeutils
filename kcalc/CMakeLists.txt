include_directories(
    ${KDE1_INCLUDE_DIR}
)

set(KCALC_SRCS
    kcalc.cpp kcalc_core.cpp configdlg.cpp fontdlg.cpp dlabel.cpp stats.cpp
)

set(KCALC_MOC
    kcalc.moc configdlg.moc fontdlg.moc dlabel.moc
)

QT1_WRAP_MOC(MOC_FILES ${KCALC_MOC})

add_executable(kcalc1 ${KCALC_SRCS} ${MOC_FILES})
target_link_libraries(kcalc1
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS kcalc1 RUNTIME DESTINATION ${KDE1_BINDIR})

install(FILES kcalc.kdelnk DESTINATION ${KDE1_APPSDIR}/Utilities)

install_icon(kcalc.xpm "${KDE1_ICONDIR}")
install_icon(kcalclogo.xpm "${KDE1_DATADIR}/kcalc")

install(FILES
    doc/index-1.html
    doc/index-2.html
    doc/index-3.html
    doc/index-4.html
    doc/index-5.html
    doc/index-6.html
    doc/index-7.html
    doc/index-8.html
    doc/index-9.html
    doc/index.html
    doc/logotp3.gif
    doc/kcalc_on_Aix.txt
    doc/kcalc_on_OSF.txt
    DESTINATION ${KDE1_HTMLDIR}/en/kcalc)

install(FILES
    doc/de/index-1.html
    doc/de/index-2.html
    doc/de/index-3.html
    doc/de/index-4.html
    doc/de/index-5.html
    doc/de/index-6.html
    doc/de/index-7.html
    doc/de/index-8.html
    doc/de/index-9.html
    doc/de/index.html
    doc/de/logotp3.gif
    DESTINATION ${KDE1_HTMLDIR}/de/kcalc)
