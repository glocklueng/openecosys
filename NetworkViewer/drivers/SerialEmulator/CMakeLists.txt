

MESSAGE( STATUS "Buiding SerialEmulator driver...")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

SET(SerialEmulator_SRCS
	SerialEmulator.cpp
)

SET(SerialEmulator_HEADERS 
	SerialEmulator.h
)

SET(SerialEmulator_UIS
	
)

SET(SerialEmulator_QRC 
	
)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(SerialEmulator_QRC_SRCS ${SerialEmulator_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(SerialEmulator_UI_SRCS ${SerialEmulator_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(SerialEmulator_MOC_SRCS ${SerialEmulator_HEADERS})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/QextSerialPort/src ${PROJECT_SOURCE_DIR}/drivers ${QT_INCLUDES} )

add_library(SerialEmulatorDriver SHARED ${SerialEmulator_SRCS} ${SerialEmulator_HEADERS} ${SerialEmulator_QRC_SRCS} ${SerialEmulator_MOC_SRCS} ${SerialEmulator_UI_SRCS}  )
target_link_libraries(SerialEmulatorDriver  QextSerialPort networkViewerBase ${QT_LIBRARIES} )
set_target_properties(SerialEmulatorDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS SerialEmulatorDriver DESTINATION drivers/SerialEmulator)
