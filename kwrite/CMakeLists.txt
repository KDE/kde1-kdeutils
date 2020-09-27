include_directories(
    ${KDE1_INCLUDE_DIR}
)

set(KWRITE_SRCS
    highlight.cpp kmimemagic.cpp kwdialog.cpp kwdoc.cpp kwview.cpp toplevel.cpp
)

set(KWRITE_MOC
    highlight.moc kmimemagic.moc kwdialog.moc kwdoc.moc kwview.moc toplevel.moc
)

QT1_WRAP_MOC(MOC_FILES ${KWRITE_MOC})

add_executable(kwrite1 ${KWRITE_SRCS} ${MOC_FILES})
target_link_libraries(kwrite1
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KFM}
    ${KDE1_KFILE}
)

install(TARGETS kwrite1 RUNTIME DESTINATION ${KDE1_BINDIR})

install(FILES kwrite.kdelnk DESTINATION ${KDE1_DATADIR}/kwrite)

install_icon(kwrite.xpm "${KDE1_DATADIR}/kwrite")
install_icon(redo.xpm "${KDE1_DATADIR}/kwrite")
install_icon(undo.xpm "${KDE1_DATADIR}/kwrite")
