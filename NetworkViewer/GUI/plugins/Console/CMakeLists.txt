MESSAGE( STATUS "Buiding Console plugin...")

include(${QT_USE_FILE})

SET(Plugin_SRCS
	ConsolePlugin.cpp
)

SET(Plugin_HEADERS 
	ConsolePlugin.h
)

SET(Plugin_UIS
	ConsolePlugin.ui
)

SET(Plugin_QRC 
	
)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(Plugin_QRC_SRCS ${Plugin_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(Plugin_UI_SRCS ${Plugin_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(Plugin_MOC_SRCS ${Plugin_HEADERS})

INCLUDE_DIRECTORIES(${PLUGIN_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_library(ConsolePlugin SHARED ${Plugin_SRCS} ${Plugin_HEADERS} ${Plugin_QRC_SRCS} ${Plugin_MOC_SRCS} ${Plugin_UI_SRCS}  )
target_link_libraries(ConsolePlugin  ${QT_LIBRARIES} ${PLUGIN_LIBRARIES})
IF(WIN32)
	set_target_properties(ConsolePlugin PROPERTIES LINK_FLAGS "--enable-auto-import")
ENDIF(WIN32)
INSTALL(TARGETS ConsolePlugin DESTINATION plugins/console)	

