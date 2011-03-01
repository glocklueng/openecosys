

MESSAGE( STATUS "Buiding SerialBridge driver...")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

SET(SerialBridge_SRCS
	SerialBridge.cpp
)

SET(SerialBridge_HEADERS 
	SerialBridge.h
)

SET(SerialBridge_UIS
	
)

SET(SerialBridge_QRC 
	
)


# generate rules for building source files from the resources
QT4_ADD_RESOURCES(SerialBridge_QRC_SRCS ${SerialBridge_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(SerialBridge_UI_SRCS ${SerialBridge_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(SerialBridge_MOC_SRCS ${SerialBridge_HEADERS})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/QextSerialPort/src ${PROJECT_SOURCE_DIR}/drivers  ${PROJECT_SOURCE_DIR}/drivers/SerialBridge ${QT_INCLUDES} )

add_library(SerialBridgeDriver SHARED ${SerialBridge_SRCS} ${SerialBridge_HEADERS} ${SerialBridge_QRC_SRCS} ${SerialBridge_MOC_SRCS} ${SerialBridge_UI_SRCS}  )
target_link_libraries(SerialBridgeDriver  networkViewerBase QextSerialPort ${QT_LIBRARIES} )
set_target_properties(SerialBridgeDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS SerialBridgeDriver DESTINATION drivers/SerialBridge)
