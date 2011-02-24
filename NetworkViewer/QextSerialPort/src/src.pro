PROJECT                 = qextserialport
TEMPLATE                = lib
VERSION                 = 1.2.0
DESTDIR                 = build
CONFIG                 += qt warn_on debug_and_release
CONFIG                  += dll
DEFINES                 += QEXTSERIALPORT_LIB
#CONFIG                 += staticlib

# event driven device enumeration on windows requires the gui module
!win32:QT               -= gui

OBJECTS_DIR             = tmp
MOC_DIR                 = tmp
DEPENDDIR               = .
INCLUDEDIR              = .
HEADERS                 = qextportinfo.h \
                          qextportsettings.h \
                          qextserialport.h \
                          qextserialenumerator.h \
                          qextserialhandshake.h \
                          qextserialport_global.h
SOURCES                 = qextportinfo.cpp \
                          qextportsettings.cpp \
                          qextserialport.cpp \
                          qextserialhandshake.cpp

unix:SOURCES           += posix_qextserialport.cpp
unix:!macx:SOURCES     += qextserialenumerator_unix.cpp
macx {
  SOURCES          += qextserialenumerator_osx.cpp
  LIBS             += -framework IOKit -framework CoreFoundation
}

win32 {
  SOURCES          += win_qextserialport.cpp qextserialenumerator_win.cpp
  LIBS             += -lsetupapi -ladvapi32 -luser32
}

CONFIG(debug, debug|release) {
    TARGET = qextserialportd
} else {
    TARGET = qextserialport
}
