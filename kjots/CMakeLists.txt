include_directories(
    ${KDE1_INCLUDE_DIR}
)

add_definitions(-Dlinux)

set(kjots_SRCS
    main.cpp
    KJotsMain.cpp
    SubjList.cpp
    ReadListConf.cpp
    cpopmenu.cpp
    confdialog.cpp
)

set(kjots_MOC
    confdialog.moc
    cpopmenu.moc
    KJotsMain.moc
    SubjList.moc
)

QT1_WRAP_MOC(MOC_FILES ${kjots_MOC})

add_executable(kjots1 ${kjots_SRCS} ${MOC_FILES})
target_link_libraries(kjots1
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KFILE}
)

install(TARGETS kjots1 RUNTIME DESTINATION ${KDE1_BINDIR})
install_icon(icons/kjots.xpm "${KDE1_ICONDIR}")
install(FILES Kjots.kdelnk DESTINATION ${KDE1_APPSDIR}/Utilities)
add_subdirectory(icons)

install(FILES
    doc/index.html
    doc/index-1.html
    doc/index-2.html
    doc/index-3.html
    doc/logotp3.gif
    DESTINATION ${KDE1_HTMLDIR}/en/kjots)
