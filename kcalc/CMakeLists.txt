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

install(FILES kcalc.kdelnk DESTINATION ${KDE1_DATADIR}/kcalc)

install_icon(kcalc.xpm "${KDE1_DATADIR}/kcalc")
install_icon(kcalclogo.xpm "${KDE1_DATADIR}/kcalc")
