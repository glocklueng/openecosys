#core/src
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTXML TRUE)

include(${QT_USE_FILE})

SET(NETV_CORE_SRCS
    NETVInterfaceHandler.cpp
    ModuleVariable.cpp
    ModuleConfiguration.cpp
    NetworkModule.cpp
    NetworkScheduler.cpp
    NETVDevice.cpp
    UserPreferences.cpp
    NETVInterfaceManager.cpp
    NETVRemoteClient.cpp
    NETVRemoteServer.cpp
    NETVBroadcaster.cpp
)

SET(NETV_CORE_HEADERS
    ModuleConfiguration.h
    ModuleVariable.h
    NETV_define.h
    NETVDevice.h
    NETVInterfaceHandler.h
    NETVInterfaceManager.h
    NETVMessageEvent.h
    NetworkModule.h
    NetworkScheduler.h
    UserPreferences.h
    NETVRemoteClient.h
    NETVRemoteServer.h
    NETVBroadcaster.h
)




#This will generate moc_* for Qt
QT4_WRAP_CPP(NETV_CORE_MOC_SRCS ${NETV_CORE_HEADERS})

#Include directories
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}
                ${PROJECT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}
                ${QWT_SRC_DIR}
                ${PROJECT_SOURCE_DIR}/core/src
)


#NetworkViewer Library
add_library(networkViewerCore SHARED ${NETV_CORE_SRCS} ${NETV_CORE_MOC_SRCS} ${NETV_CORE_HEADERS})
target_link_libraries(networkViewerCore ${QT_LIBRARIES})
INSTALL(TARGETS networkViewerCore DESTINATION bin)
INSTALL(TARGETS networkViewerCore DESTINATION lib EXPORT networkViewerCore-export-targets)
INSTALL(EXPORT networkViewerCore-export-targets DESTINATION share/cmake)
