########################
# Configurations
########################
find_package(Qt4 REQUIRED)

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTXML TRUE)

include(${QT_USE_FILE})

SET(Plugin_SRCS
	SamplePlugin.cpp
)

SET(Plugin_HEADERS 
	SamplePlugin.h
)

SET(Plugin_UIS
	SamplePlugin.ui
)

SET(Plugin_QRC 
	
)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(Plugin_QRC_SRCS ${Plugin_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(Plugin_UI_SRCS ${Plugin_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(Plugin_MOC_SRCS ${Plugin_HEADERS})

INCLUDE_DIRECTORIES(./ ${PLUGIN_INCLUDES} )

add_library(SamplePlugin SHARED ${Plugin_SRCS} ${Plugin_HEADERS} ${Plugin_QRC_SRCS} ${Plugin_MOC_SRCS} ${Plugin_UI_SRCS}  )
target_link_libraries(SamplePlugin  ${QT_LIBRARIES} ${PLUGIN_LIBRARIES})
set_target_properties(SamplePlugin PROPERTIES LINK_FLAGS "--enable-auto-import")
INSTALL(TARGETS SamplePlugin DESTINATION plugins/sample)	

