# FIXME: at least use `uname -r`
#set(KERNEL_INCLUDE_DIR /usr/src/kernels/4.8.3-300.fc25.x86_64)

include_directories(
    ${KDE1_INCLUDE_DIR}
#    ${KERNEL_INCLUDE_DIR}/include
#    ${KERNEL_INCLUDE_DIR}/arch/x86/include/
)

set(KFLOPPY_SRCS
    main.cpp about.cpp floppy.cpp mount.c
)

set(KFLOPPY_MOC
    about.moc floppy.moc
)

QT1_WRAP_MOC(KFLOPPY_MOC_FILES ${KFLOPPY_MOC})

add_executable(kfloppy ${KFLOPPY_SRCS} ${KFLOPPY_MOC_FILES})
target_link_libraries(kfloppy
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS kfloppy RUNTIME DESTINATION ${KDE1_BINDIR})

install(FILES KFloppy.kdelnk DESTINATION ${KDE1_DATADIR}/kfloppy)



install_icon(kfloppy.xpm "${KDE1_DATADIR}/kfloppy")
install_icon(kfloppylogo.xpm "${KDE1_DATADIR}/kfloppy")

######################### Next target ########################

#set(KMKDOSFS_SRCS kmkdosfs.c)
#add_executable(kmkdosfs ${KMKDOSFS_SRCS})

######################## Next target ########################

#set(KFDFORMAT_SRCS kfdformat.c)
#add_executable(kfdformat ${KFDFORMAT_SRCS})

