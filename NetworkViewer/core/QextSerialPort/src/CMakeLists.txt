

MESSAGE( STATUS "Buiding QextSerialPort")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTCORE TRUE)

include(${QT_USE_FILE})

SET(QextSerialPort_SRCS
    qextserialenumerator.cpp
    qextserialport.cpp
)

SET(QextSerialPort_HEADERS
    qextserialenumerator.h
    qextserialenumerator_p.h
    qextserialport.h
    qextserialport_p.h
    qextserialport_global.h
    qextwineventnotifier_p.h
)

SET(QextSerialPort_UIS

)

SET(QextSerialPort_QRC

)

SET(QextSerialPort_External_Libraries

)

ADD_DEFINITIONS(-DQEXTSERIALPORT_BUILD_SHARED)


#POSIX/UNIX SPECIFIC FILES & DEFINES
IF (UNIX AND NOT APPLE)
    SET(QextSerialPort_SRCS ${QextSerialPort_SRCS} qextserialenumerator_unix.cpp qextserialenumerator_linux.cpp )
    SET(QextSerialPort_External_Libraries -ludev)
ENDIF(UNIX AND NOT APPLE)

IF (APPLE)
    SET(QextSerialPort_SRCS ${QextSerialPort_SRCS} qextserialport_unix.cpp qextserialenumerator_osx.cpp)
    SET(QextSerialPort_External_Libraries "-framework CoreFoundation -framework IOKit")
ENDIF(APPLE)

#WINDOWS SPECIFIC FILES	& DEFINES
IF (WIN32)
    ADD_DEFINITIONS(-DQESP_NO_QT4_PRIVATE)
    SET(QextSerialPort_SRCS ${QextSerialPort_SRCS} qextserialport_win.cpp qextserialenumerator_win.cpp qextwineventnotifier_p.cpp)
    SET(QextSerialPort_External_Libraries -lsetupapi -ladvapi32 -luser32)
ENDIF(WIN32)




# generate rules for building source files from the resources
QT4_ADD_RESOURCES(QextSerialPort_QRC_SRCS ${QextSerialPort_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(QextSerialPort_UI_SRCS ${QextSerialPort_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(QextSerialPort_MOC_SRCS ${QextSerialPort_HEADERS})

INCLUDE_DIRECTORIES(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_library(QextSerialPort SHARED ${QextSerialPort_SRCS} ${QextSerialPort_HEADERS} ${QextSerialPort_QRC_SRCS} ${QextSerialPort_MOC_SRCS} ${QextSerialPort_UI_SRCS}  )
target_link_libraries(QextSerialPort ${QT_LIBRARIES} ${QextSerialPort_External_Libraries})
IF(WIN32)
	set_target_properties(QextSerialPort PROPERTIES LINK_FLAGS "--enable-auto-import")
ENDIF(WIN32)
INSTALL(TARGETS QextSerialPort DESTINATION bin)
INSTALL(TARGETS QextSerialPort DESTINATION lib)

