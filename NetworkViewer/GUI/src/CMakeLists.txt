#GUI/src
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTDECLARATIVE TRUE)

include(${QT_USE_FILE})

SET(NetworkViewer_UIS
	ModuleConfigurationView.ui
	NetworkView.ui
	TestView.ui
	DeviceSelectorDialog.ui
	PreferencesDialog.ui
	AddVariableDialog.ui
	NETVInterfaceManagerView.ui
)

SET(NetworkViewer_SRCS
	BasePlugin.cpp
	BasePluginEvent.cpp
	ModuleConfigurationView.cpp
	NetworkView.cpp
	NetworkModuleItem.cpp
	DeviceSelectorDialog.cpp
	ModuleVariableTableWidget.cpp
	PreferencesDialog.cpp
	AddVariableDialog.cpp
	NETVInterfaceManagerView.cpp
)

SET(NetworkViewer_HEADERS 
	BasePlugin.h
	BasePluginEvent.h
	ModuleConfigurationView.h
	NetworkView.h
	NetworkModuleItem.h
	DeviceSelectorDialog.h
	NetworkViewerSplashScreen.h
	ModuleVariableTableWidget.h
	PreferencesDialog.h
	AddVariableDialog.h
	NETVInterfaceManagerView.h
)

SET(qrc 
	NetworkViewer.qrc
)


# generate rules for building source files from the resources
QT4_ADD_RESOURCES(SRCS_QRC ${qrc})

SET (QTVERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")

#This will generate ui_* for Qt
QT4_WRAP_UI(NetworkViewer_UI_SRCS ${NetworkViewer_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(NetworkViewer_MOC_SRCS ${NetworkViewer_HEADERS})

#Include directories
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}
                ${PROJECT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}
                ${QWT_SRC_DIR}
				${PROJECT_SOURCE_DIR}/core/src
                )


#Install headers
INSTALL( FILES ${NetworkViewer_HEADERS} DESTINATION include)
INSTALL( FILES ${NetworkViewer_UI_SRCS} DESTINATION include/ui)


#NetworkViewer Executable
add_executable(NetworkViewer WIN32 main.cpp ${NetworkViewer_SRCS} ${NetworkViewer_MOC_SRCS} ${NetworkViewer_UI_SRCS} ${SRCS_QRC} ${NetworkViewer_HEADERS})
set_target_properties(NetworkViewer PROPERTIES LINK_FLAGS "--enable-auto-import")
#TODO ADD CORE LIB LINK
target_link_libraries(NetworkViewer qwt-wrap networkViewerBase ${QT_LIBRARIES})
INSTALL(TARGETS NetworkViewer DESTINATION bin)

