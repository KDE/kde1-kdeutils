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

add_executable(klipper ${klipper_SRCS} ${MOC_FILES})
target_link_libraries(klipper
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS klipper RUNTIME DESTINATION ${KDE1_BINDIR})
install_icon(klipper.xpm "${KDE1_DATADIR}/klipper")
install(FILES klipper.kdelnk DESTINATION ${KDE1_DATADIR}/klipper)
