#GUI/src
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTDECLARATIVE TRUE)

include(${QT_USE_FILE})


#Include directories
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}
                ${PROJECT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}
                ${QWT_SRC_DIR}
                ${PROJECT_SOURCE_DIR}/core/src
                )

#CREATE THE SHARED LIBRARY FOR PLUGINS
SET(NetworkViewer_Base_UIS
    NetworkView.ui
    ModuleConfigurationView.ui
    TestView.ui
    DeviceSelectorDialog.ui
    PreferencesDialog.ui
    AddVariableDialog.ui
    NETVInterfaceManagerView.ui
)

SET(NetworkViewer_Base_SRCS
    BasePlugin.cpp
    BasePluginEvent.cpp
    NetworkView.cpp
    ModuleConfigurationView.cpp
    NetworkModuleItem.cpp
    DeviceSelectorDialog.cpp
    PreferencesDialog.cpp
    AddVariableDialog.cpp
    NETVInterfaceManagerView.cpp
)

SET(NetworkViewer_Base_HEADERS
    BasePlugin.h
    BasePluginEvent.h
    NetworkView.h
    ModuleConfigurationView.h
    NetworkModuleItem.h
    DeviceSelectorDialog.h
    NetworkViewerSplashScreen.h
    PreferencesDialog.h
    AddVariableDialog.h
    NETVInterfaceManagerView.h
)


SET(NetworkViewer_Base_QRC
    NetworkViewer.qrc
)

#Generate rules for building source files from the resources
QT4_ADD_RESOURCES(SRCS_QRC ${NetworkViewer_Base_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(NetworkViewer_Base_UI_SRCS ${NetworkViewer_Base_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(NetworkViewer_Base_MOC_SRCS ${NetworkViewer_Base_HEADERS})

#NetworkViewer Base Library
add_library(networkViewerBase SHARED ${NetworkViewer_Base_SRCS} ${NetworkViewer_Base_HEADERS} ${NetworkViewer_Base_MOC_SRCS} ${NetworkViewer_Base_UI_SRCS} ${SRCS_QRC})
target_link_libraries(networkViewerBase networkViewerCore ${QT_LIBRARIES})
INSTALL(TARGETS networkViewerBase DESTINATION bin)
INSTALL(TARGETS networkViewerBase DESTINATION lib)

#FIXME EXPORTS
#INSTALL(TARGETS networkViewerBase DESTINATION lib EXPORT networkViewerBase-export-targets)
#INSTALL(EXPORT networkViewerBase-export-targets DESTINATION share/cmake)

#Install headers
INSTALL( FILES ${NetworkViewer_Base_HEADERS} DESTINATION include)
INSTALL( FILES ${NetworkViewer_Base_UI_SRCS} DESTINATION include/ui)


#APPLICATION

#NetworkViewer Executable
add_executable(NetworkViewer WIN32 main.cpp)
IF(WIN32)
	set_target_properties(NetworkViewer PROPERTIES LINK_FLAGS "--enable-auto-import")
ENDIF(WIN32)
target_link_libraries(NetworkViewer networkViewerCore networkViewerBase ${QT_LIBRARIES})
INSTALL(TARGETS NetworkViewer DESTINATION bin)

