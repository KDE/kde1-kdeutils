include_directories(
    ${KDE1_INCLUDE_DIR}
)

add_definitions(-Dlinux)

set(KTOP_SRCS ktop.cpp MainMenu.cpp ProcessMenu.cpp TaskMan.cpp ProcListPage.cpp
    IconList.cpp ProcessList.cpp OSProcessList.cpp OSProcess.cpp
    PerfMonPage.cpp FancyPlotter.cpp MultiMeter.cpp SignalPlotter.cpp
    OSStatus.cpp ReniceDlg.cpp
)

set(KTOP_MOC ktop.moc MainMenu.moc ProcessMenu.moc TaskMan.moc ProcListPage.moc
    ProcessList.moc PerfMonPage.moc FancyPlotter.moc MultiMeter.moc
    SignalPlotter.moc ReniceDlg.moc
)

QT1_WRAP_MOC(MOC_FILES ${KTOP_MOC})

add_executable(ktop ${KTOP_SRCS} ${MOC_FILES})
target_link_libraries(ktop
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KFM}
)

install(TARGETS ktop RUNTIME DESTINATION ${KDE1_BINDIR})

install(FILES ktop.kdelnk DESTINATION ${KDE1_APPSDIR}/System)

install_icon(ktop.xpm "${KDE1_DATADIR}/ktop")
