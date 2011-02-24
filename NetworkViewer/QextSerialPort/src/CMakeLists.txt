

MESSAGE( STATUS "Buiding QextSerialPort")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTCORE TRUE)

include(${QT_USE_FILE})

SET(QextSerialPort_SRCS
    qextportinfo.cpp
    qextportsettings.cpp
    qextserialport.cpp
    qextserialhandshake.cpp
)

SET(QextSerialPort_HEADERS
    qextportinfo.h
    qextportsettings.h
    qextserialport.h
    qextserialenumerator.h
    qextserialhandshake.h
    qextserialport_global.h
)

SET(QextSerialPort_UIS

)

SET(QextSerialPort_QRC

)

SET(QextSerialPort_External_Libraries

)

ADD_DEFINITIONS(-DQEXTSERIALPORT_LIB)


#POSIX/UNIX SPECIFIC FILES & DEFINES
IF (UNIX AND NOT APPLE)
    SET(QextSerialPort_SRCS ${QextSerialPort_SRCS} posix_qextserialport.cpp qextserialenumerator_unix.cpp)
ENDIF(UNIX AND NOT APPLE)

IF (APPLE)
    SET(QextSerialPort_SRCS ${QextSerialPort_SRCS} posix_qextserialport.cpp qextserialenumerator_osx.cpp)
    SET(QextSerialPort_External_Libraries "-framework CoreFoundation -framework IOKit")
ENDIF(APPLE)

#WINDOWS SPECIFIC FILES	& DEFINES
IF (WIN32)
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_TTY_WIN_)
    ADD_DEFINITIONS(-DWINVER=0x0501)
    SET(QextSerialPort_SRCS ${QextSerialPort_SRCS} win_qextserialport.cpp qextserialenumerator_win.cpp)
    SET(QextSerialPort_External_Libraries -lsetupapi -ladvapi32 -luser32)
ENDIF(WIN32)



# generate rules for building source files from the resources
QT4_ADD_RESOURCES(QextSerialPort_QRC_SRCS ${QextSerialPort_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(QextSerialPort_UI_SRCS ${QextSerialPort_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(QextSerialPort_MOC_SRCS ${QextSerialPort_HEADERS})

INCLUDE_DIRECTORIES(${QT_INCLUDES} )

add_library(QextSerialPort SHARED ${QextSerialPort_SRCS} ${QextSerialPort_HEADERS} ${QextSerialPort_QRC_SRCS} ${QextSerialPort_MOC_SRCS} ${QextSerialPort_UI_SRCS}  )
target_link_libraries(QextSerialPort ${QT_LIBRARIES} ${QextSerialPort_External_Libraries})
set_target_properties(QextSerialPort PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS QextSerialPort DESTINATION bin)
