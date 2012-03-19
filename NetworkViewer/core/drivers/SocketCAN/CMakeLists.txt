

MESSAGE( STATUS "Buiding SocketCAN driver...")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

SET(SocketCAN_SRCS
	SocketCAN.cpp
)

SET(SocketCAN_HEADERS 
	SocketCAN.h
)

SET(SocketCAN_UIS
	
)

SET(SocketCAN_QRC 
	
)


# generate rules for building source files from the resources
QT4_ADD_RESOURCES(SocketCAN_QRC_SRCS ${SocketCAN_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(SocketCAN_UI_SRCS ${SocketCAN_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(SocketCAN_MOC_SRCS ${SocketCAN_HEADERS})

INCLUDE_DIRECTORIES(${DRIVER_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_library(SocketCANDriver SHARED ${SocketCAN_SRCS} ${SocketCAN_HEADERS} ${SocketCAN_QRC_SRCS} ${SocketCAN_MOC_SRCS} ${SocketCAN_UI_SRCS}  )
target_link_libraries(SocketCANDriver  ${DRIVER_LIBRARIES} )
IF(WIN32)
	set_target_properties(SocketCANDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
ENDIF(WIN32)
INSTALL(TARGETS SocketCANDriver DESTINATION drivers/SocketCAN)
