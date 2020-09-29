include_directories(
    ${KDE1_INCLUDE_DIR}
)

set(KHEXDIT_SRCS
    hexdata.cpp
    hexfile.cpp
    hexwidget.cpp
)

set(KHEXDIT_MOC
    hexdata.h
    hexfile.h
    hexwidget.h
)

QT1_WRAP_MOC(MOC_FILES ${KHEXDIT_MOC})

add_executable(khexdit ${KHEXDIT_SRCS} ${MOC_FILES})
target_link_libraries(khexdit
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KFM}
    ${KDE1_KFILE}
)

install(TARGETS khexdit RUNTIME DESTINATION ${KDE1_BINDIR})

install(FILES khexdit.kdelnk DESTINATION ${KDE1_APPSDIR}/Utilities)

install_icon(khexdit.xpm "${KDE1_ICONDIR}")

install(FILES doc/en/index.html DESTINATION ${KDE1_HTMLDIR}/en/khexdit)
install(FILES doc/it/index.html DESTINATION ${KDE1_HTMLDIR}/it/khexdit)
install(FILES doc/no/index.html DESTINATION ${KDE1_HTMLDIR}/no/khexdit)
