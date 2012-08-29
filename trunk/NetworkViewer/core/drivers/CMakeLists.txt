########################
# Configurations
########################

SET(DRIVER_INCLUDES
        ${PROJECT_SOURCE_DIR}/core/src
        ${PROJECT_SOURCE_DIR}/core/QextSerialPort/src
        ${PROJECT_SOURCE_DIR}/drivers
        ${QT_INCLUDES}
)

SET(DRIVER_LIBRARIES
        networkViewerCore
        ${QT_LIBRARIES}
)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/drivers)

ADD_SUBDIRECTORY(SerialBridge)
ADD_SUBDIRECTORY(EthernetBridge)
ADD_SUBDIRECTORY(SerialEmulator)
ADD_SUBDIRECTORY(Loopback)
ADD_SUBDIRECTORY(TcpRemoteClient)


IF (NOT APPLE)
    ADD_SUBDIRECTORY(PCAN)
ENDIF (NOT APPLE)

IF(UNIX AND NOT APPLE)
	ADD_SUBDIRECTORY(SocketCAN)
ENDIF(UNIX AND NOT APPLE)


