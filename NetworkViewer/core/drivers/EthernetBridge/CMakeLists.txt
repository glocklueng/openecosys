

MESSAGE( STATUS "Buiding EthernetBridge driver...")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

SET(EthernetBridge_SRCS
	EthernetBridge.cpp
)

SET(EthernetBridge_HEADERS 
	EthernetBridge.h
)

SET(EthernetBridge_UIS
	
)

SET(EthernetBridge_QRC 
	
)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(EthernetBridge_QRC_SRCS ${EthernetBridge_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(EthernetBridge_UI_SRCS ${EthernetBridge_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(EthernetBridge_MOC_SRCS ${EthernetBridge_HEADERS})

INCLUDE_DIRECTORIES(${DRIVER_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_library(EthernetBridgeDriver SHARED ${EthernetBridge_SRCS} ${EthernetBridge_HEADERS} ${EthernetBridge_QRC_SRCS} ${EthernetBridge_MOC_SRCS} ${EthernetBridge_UI_SRCS}  )
target_link_libraries(EthernetBridgeDriver  ${DRIVER_LIBRARIES} ${QT_LIBRARIES} )
set_target_properties(EthernetBridgeDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS EthernetBridgeDriver DESTINATION drivers/EthernetBridge)
