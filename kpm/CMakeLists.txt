include_directories(
    ${KDE1_INCLUDE_DIR}
)

add_definitions(-Dlinux)

set(kpmdocked_SRCS
    docked.C
    )

set(kpm_SRCS
    dialogs.C
    infobar.C
    pstable.C
    svec.C
    wchan.C
    fieldsel.C
    prefs.C
    qps.C
    ttystr.C
    details.C
    htable.C
    proc.C
    scheddlg.C
    uidstr.C
    moc.C
    buttongroup.C
)

set(kpm_MOC
    details.moc
    dialogs.moc
    fieldsel.moc
    htable.moc
    infobar.moc
    prefs.moc
    pstable.moc
    qps.moc
    scheddlg.moc
)

set(kpm_DOC
    docs/en/kpm.html
    docs/en/kpm-1.html
    docs/en/kpm-2.html
    docs/en/kpm-3.html
    docs/en/kpm-4.html
    docs/en/logotp3.gif
)

QT1_WRAP_MOC(MOC_FILES ${kpm_MOC})
QT1_WRAP_MOC(DOCKED_MOC_FILES docked.moc)

add_executable(kpmdocked ${kpmdocked_SRCS} ${DOCKED_MOC_FILES})

add_executable(kpm ${kpm_SRCS} ${MOC_FILES})
target_link_libraries(kpm
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)
target_link_libraries(kpmdocked
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS kpm RUNTIME DESTINATION ${KDE1_BINDIR})
install_icon(icons/kpm.xpm "${KDE1_DATADIR}/kpm")
install(FILES kpm.kdelnk DESTINATION ${KDE1_DATADIR}/kpm)

install(FILES ${kpm_DOC} DESTINATION ${KDE1_HTMLDIR}/en/kpm)
