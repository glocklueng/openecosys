

MESSAGE( STATUS "Buiding Loopback driver...")

find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

SET(Loopback_SRCS
	Loopback.cpp
)

SET(Loopback_HEADERS 
	Loopback.h
)

SET(Loopback_UIS
	
)

SET(Loopback_QRC 
	
)


# generate rules for building source files from the resources
QT4_ADD_RESOURCES(Loopback_QRC_SRCS ${Loopback_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(Loopback_UI_SRCS ${Loopback_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(Loopback_MOC_SRCS ${Loopback_HEADERS})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ 
${PROJECT_SOURCE_DIR}/drivers  
${PROJECT_SOURCE_DIR}/driver/Loopback
${QT_INCLUDES} )

add_library(LoopbackDriver SHARED ${Loopback_SRCS} ${Loopback_HEADERS} ${Loopback_QRC_SRCS} ${Loopback_MOC_SRCS} ${Loopback_UI_SRCS}  )
target_link_libraries(LoopbackDriver  networkViewerBase ${QT_LIBRARIES} )
set_target_properties(LoopbackDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS LoopbackDriver DESTINATION drivers/Loopback)
