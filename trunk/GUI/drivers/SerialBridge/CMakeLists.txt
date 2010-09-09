

MESSAGE( STATUS "Buiding SerialBridge driver...")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

SET(SerialBridge_SRCS
	SerialBridge.cpp
        qextserialbase.cpp
        qextserialport.cpp
        qextserialenumerator.cpp
)

SET(SerialBridge_HEADERS 
	SerialBridge.h
        qextserialenumerator.h
        qextserialbase.h
        qextserialport.h
)

SET(SerialBridge_UIS
	
)

SET(SerialBridge_QRC 
	
)


#POSIX/UNIX SPECIFIC FILES & DEFINES
IF (UNIX)
    ADD_DEFINITIONS(-D_TTY_POSIX_)
        SET(SerialBridge_SRCS ${SerialBridge_SRCS} posix_qextserialport.cpp)
        SET(SerialBridge_HEADERS ${SerialBridge_HEADERS} posix_qextserialport.h)
ENDIF(UNIX)

#WINDOWS SPECIFIC FILES	& DEFINES
IF (WIN32)
        SET(SerialBridge_SRCS ${SerialBridge_SRCS} win_qextserialport.cpp)
        SET(SerialBridge_HEADERS ${SerialBridge_HEADERS} win_qextserialport.h)
ENDIF(WIN32)



# generate rules for building source files from the resources
QT4_ADD_RESOURCES(SerialBridge_QRC_SRCS ${SerialBridge_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(SerialBridge_UI_SRCS ${SerialBridge_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(SerialBridge_MOC_SRCS ${SerialBridge_HEADERS})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/drivers  ${PROJECT_SOURCE_DIR}/drivers/SerialBridge ${QT_INCLUDES} )

add_library(SerialBridgeDriver SHARED ${SerialBridge_SRCS} ${SerialBridge_HEADERS} ${SerialBridge_QRC_SRCS} ${SerialBridge_MOC_SRCS} ${SerialBridge_UI_SRCS}  )
target_link_libraries(SerialBridgeDriver  networkViewerBase ${QT_LIBRARIES} )
set_target_properties(SerialBridgeDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS SerialBridgeDriver DESTINATION drivers/SerialBridge)
