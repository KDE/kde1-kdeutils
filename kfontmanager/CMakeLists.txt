include_directories(
    ${KDE1_INCLUDE_DIR}
)

add_definitions(-Dlinux)

set(kfontmanager_SRCS
    main.cpp kfontmanager.cpp kfontlist.cpp kfontexplorer.cpp
)

set(kfontmanager_MOC
    kfontexplorer.moc
    kfontlist.moc
    kfontmanager.moc
)

QT1_WRAP_MOC(MOC_FILES ${kfontmanager_MOC})

add_executable(kfontmanager ${kfontmanager_SRCS} ${MOC_FILES})
target_link_libraries(kfontmanager
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS kfontmanager RUNTIME DESTINATION ${KDE1_BINDIR})
install_icon(kfontmanager.xpm "${KDE1_ICONDIR}")
