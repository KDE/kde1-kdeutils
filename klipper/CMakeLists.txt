include_directories(
    ${KDE1_INCLUDE_DIR}
)

add_definitions(-Dlinux)

set(klipper_SRCS
    main.cpp toplevel.cpp
)

set(klipper_MOC
    toplevel.moc
)

QT1_WRAP_MOC(MOC_FILES ${klipper_MOC})

add_executable(klipper1 ${klipper_SRCS} ${MOC_FILES})
target_link_libraries(klipper1
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS klipper1 RUNTIME DESTINATION ${KDE1_BINDIR})
install_icon(klipper.xpm "${KDE1_ICONDIR}")
install(FILES klipper.kdelnk DESTINATION ${KDE1_APPSDIR}/Utilities)
