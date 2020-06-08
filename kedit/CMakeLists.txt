include_directories(
    ${KDE1_INCLUDE_DIR}
)

set(KEDIT_SRCS
    filldlg.cpp  keditcl1.cpp  keditcl2.cpp  kedit.cpp  mail.cpp  print.cpp  urldlg.cpp
)

set(KEDIT_MOC
    filldlg.moc kedit.moc keditcl.moc mail.moc print.moc urldlg.moc
)

QT1_WRAP_MOC(MOC_FILES ${KEDIT_MOC})

add_executable(kedit ${KEDIT_SRCS} ${MOC_FILES})
target_link_libraries(kedit
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KFILE}
    ${KDE1_KFM}
    ${KDE1_KSPELL}
)

install(TARGETS kedit RUNTIME DESTINATION ${KDE1_BINDIR})

install(FILES kedit.kdelnk DESTINATION ${KDE1_DATADIR}/kedit)

add_subdirectory(pixmaps)
