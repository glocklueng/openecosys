

MESSAGE( STATUS "Buiding SerialEmulator driver...")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

SET(SerialEmulator_SRCS
    SerialEmulator.cpp
    SerialEmulatorConfigure.cpp
)

SET(SerialEmulator_HEADERS 
    SerialEmulator.h
    SerialEmulatorConfigure.h
)

SET(SerialEmulator_UIS
    SerialEmulatorConfigure.ui
)

SET(SerialEmulator_QRC 
	
)


# generate rules for building source files from the resources
QT4_ADD_RESOURCES(SerialEmulator_QRC_SRCS ${SerialEmulator_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(SerialEmulator_UI_SRCS ${SerialEmulator_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(SerialEmulator_MOC_SRCS ${SerialEmulator_HEADERS})

INCLUDE_DIRECTORIES(${DRIVER_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

MESSAGE(STATUS ${CMAKE_CURRENT_BINARY_DIR})

add_library(SerialEmulatorDriver SHARED ${SerialEmulator_SRCS} ${SerialEmulator_HEADERS} ${SerialEmulator_QRC_SRCS} ${SerialEmulator_MOC_SRCS} ${SerialEmulator_UI_SRCS}  )
target_link_libraries(SerialEmulatorDriver  QextSerialPort ${DRIVER_LIBRARIES} )
set_target_properties(SerialEmulatorDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS SerialEmulatorDriver DESTINATION drivers/SerialEmulator)
