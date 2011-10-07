

MESSAGE( STATUS "Buiding TcpRemoteClient driver...")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

SET(TcpRemoteClient_SRCS
	TcpRemoteClient.cpp
)

SET(TcpRemoteClient_HEADERS 
	TcpRemoteClient.h
)

SET(TcpRemoteClient_UIS
	
)

SET(TcpRemoteClient_QRC 
	
)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(TcpRemoteClient_QRC_SRCS ${TcpRemoteClient_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(TcpRemoteClient_UI_SRCS ${TcpRemoteClient_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(TcpRemoteClient_MOC_SRCS ${TcpRemoteClient_HEADERS})

INCLUDE_DIRECTORIES(${DRIVER_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_library(TcpRemoteClientDriver SHARED ${TcpRemoteClient_SRCS} ${TcpRemoteClient_HEADERS} ${TcpRemoteClient_QRC_SRCS} ${TcpRemoteClient_MOC_SRCS} ${TcpRemoteClient_UI_SRCS}  )
target_link_libraries(TcpRemoteClientDriver  ${DRIVER_LIBRARIES} ${QT_LIBRARIES} )
set_target_properties(TcpRemoteClientDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS TcpRemoteClientDriver DESTINATION drivers/TcpRemoteClient)
