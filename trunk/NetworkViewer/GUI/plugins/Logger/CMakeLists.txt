########################
# Configurations
########################


SET(Logger_SRCS
	Logger.cpp
        ModuleVariableTableWidget.cpp
)

SET(Logger_HEADERS 
	Logger.h
        ModuleVariableTableWidget.h
)

SET(Logger_UIS
	Logger.ui
)

SET(Logger_QRC 
	
)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(Logger_QRC_SRCS ${Logger_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(Logger_UI_SRCS ${Logger_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(Logger_MOC_SRCS ${Logger_HEADERS})

INCLUDE_DIRECTORIES(${PLUGIN_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_library(Logger SHARED ${Logger_SRCS} ${Logger_HEADERS} ${Logger_QRC_SRCS} ${Logger_MOC_SRCS} ${Logger_UI_SRCS}  )
target_link_libraries(Logger  ${QT_LIBRARIES} ${PLUGIN_LIBRARIES})
set_target_properties(Logger PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS Logger DESTINATION plugins/Logger)

