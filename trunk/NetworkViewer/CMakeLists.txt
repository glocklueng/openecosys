PROJECT (OpenECoSys-NetworkViewer)

cmake_minimum_required(VERSION 2.8)

########################
# Configurations
########################

SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "8alpha")
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})


#OSX Architecture, will build for 32 bits systems
#set(CMAKE_OSX_ARCHITECTURES "i386")



SET(CMAKE_BUILD_TYPE Release)

IF(WIN32)
	MESSAGE( STATUS "Windows version: " ${CMAKE_SYSTEM})
        SET(CMAKE_INSTALL_PREFIX "$ENV{SYSTEMDRIVE}/NetworkViewer-${CPACK_PACKAGE_VERSION}")
	MESSAGE( STATUS "Install directory is : " ${CMAKE_INSTALL_PREFIX})
ELSE(WIN32)
        SET(CMAKE_INSTALL_PREFIX "/usr/local/NetworkViewer-${CPACK_PACKAGE_VERSION}")
ENDIF(WIN32)


#finding qt
IF (WIN32)
     SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)

find_package(Qt4 REQUIRED)


INCLUDE(InstallRequiredSystemLibraries)

#This will enable to output everything in the bin directory for testing and debugging...
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)




# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTDECLARATIVE TRUE)

include(${QT_USE_FILE})

#
#This is a qwt svn external dependency.
#svn propget svn:externals to view them
#

SET(QWT_SRC_DIR ${PROJECT_SOURCE_DIR}/qwt_svn/src)

#Create a library with the sources
SET(QWT_SRCS
    #Base
    ${QWT_SRC_DIR}/qwt_abstract_scale_draw.cpp
    ${QWT_SRC_DIR}/qwt_color_map.cpp
    ${QWT_SRC_DIR}/qwt_clipper.cpp
    ${QWT_SRC_DIR}/qwt_double_interval.cpp
    ${QWT_SRC_DIR}/qwt_double_rect.cpp
    ${QWT_SRC_DIR}/qwt_dyngrid_layout.cpp
    ${QWT_SRC_DIR}/qwt_layout_metrics.cpp
    ${QWT_SRC_DIR}/qwt_math.cpp
    ${QWT_SRC_DIR}/qwt_magnifier.cpp
    ${QWT_SRC_DIR}/qwt_paint_buffer.cpp
    ${QWT_SRC_DIR}/qwt_panner.cpp
    ${QWT_SRC_DIR}/qwt_painter.cpp
    ${QWT_SRC_DIR}/qwt_picker.cpp
    ${QWT_SRC_DIR}/qwt_round_scale_draw.cpp
    ${QWT_SRC_DIR}/qwt_scale_div.cpp
    ${QWT_SRC_DIR}/qwt_scale_draw.cpp
    ${QWT_SRC_DIR}/qwt_scale_map.cpp
    ${QWT_SRC_DIR}/qwt_spline.cpp
    ${QWT_SRC_DIR}/qwt_text_engine.cpp
    ${QWT_SRC_DIR}/qwt_text_label.cpp
    ${QWT_SRC_DIR}/qwt_text.cpp
    ${QWT_SRC_DIR}/qwt_event_pattern.cpp
    ${QWT_SRC_DIR}/qwt_picker_machine.cpp
    ${QWT_SRC_DIR}/qwt_scale_engine.cpp
    ${QWT_SRC_DIR}/qwt_symbol.cpp
    #QWT_PLOT
    ${QWT_SRC_DIR}/qwt_curve_fitter.cpp
    ${QWT_SRC_DIR}/qwt_data.cpp
    ${QWT_SRC_DIR}/qwt_interval_data.cpp
    ${QWT_SRC_DIR}/qwt_legend.cpp
    ${QWT_SRC_DIR}/qwt_legend_item.cpp
    ${QWT_SRC_DIR}/qwt_plot.cpp
    ${QWT_SRC_DIR}/qwt_plot_print.cpp
    ${QWT_SRC_DIR}/qwt_plot_xml.cpp
    ${QWT_SRC_DIR}/qwt_plot_axis.cpp
    ${QWT_SRC_DIR}/qwt_plot_curve.cpp
    ${QWT_SRC_DIR}/qwt_plot_dict.cpp
    ${QWT_SRC_DIR}/qwt_plot_grid.cpp
    ${QWT_SRC_DIR}/qwt_plot_item.cpp
    ${QWT_SRC_DIR}/qwt_plot_spectrogram.cpp
    ${QWT_SRC_DIR}/qwt_plot_scaleitem.cpp
    ${QWT_SRC_DIR}/qwt_plot_marker.cpp
    ${QWT_SRC_DIR}/qwt_plot_layout.cpp
    ${QWT_SRC_DIR}/qwt_plot_printfilter.cpp
    ${QWT_SRC_DIR}/qwt_plot_rasteritem.cpp
    ${QWT_SRC_DIR}/qwt_plot_canvas.cpp
    ${QWT_SRC_DIR}/qwt_plot_rescaler.cpp
    ${QWT_SRC_DIR}/qwt_plot_panner.cpp
    ${QWT_SRC_DIR}/qwt_plot_picker.cpp
    ${QWT_SRC_DIR}/qwt_plot_zoomer.cpp
    ${QWT_SRC_DIR}/qwt_plot_magnifier.cpp
    ${QWT_SRC_DIR}/qwt_raster_data.cpp
    ${QWT_SRC_DIR}/qwt_scale_widget.cpp
)

SET(QWT_HEADERS
    #Base
    ${QWT_SRC_DIR}/qwt.h
    ${QWT_SRC_DIR}/qwt_abstract_scale_draw.h
    ${QWT_SRC_DIR}/qwt_array.h
    ${QWT_SRC_DIR}/qwt_color_map.h
    ${QWT_SRC_DIR}/qwt_clipper.h
    ${QWT_SRC_DIR}/qwt_double_interval.h
    ${QWT_SRC_DIR}/qwt_double_rect.h
    ${QWT_SRC_DIR}/qwt_dyngrid_layout.h
    ${QWT_SRC_DIR}/qwt_global.h
    ${QWT_SRC_DIR}/qwt_layout_metrics.h
    ${QWT_SRC_DIR}/qwt_math.h
    ${QWT_SRC_DIR}/qwt_magnifier.h
    ${QWT_SRC_DIR}/qwt_paint_buffer.h
    ${QWT_SRC_DIR}/qwt_painter.h
    ${QWT_SRC_DIR}/qwt_panner.h
    ${QWT_SRC_DIR}/qwt_picker.h
    ${QWT_SRC_DIR}/qwt_picker_machine.h
    ${QWT_SRC_DIR}/qwt_polygon.h
    ${QWT_SRC_DIR}/qwt_round_scale_draw.h
    ${QWT_SRC_DIR}/qwt_scale_div.h
    ${QWT_SRC_DIR}/qwt_scale_draw.h
    ${QWT_SRC_DIR}/qwt_scale_engine.h
    ${QWT_SRC_DIR}/qwt_scale_map.h
    ${QWT_SRC_DIR}/qwt_spline.h
    ${QWT_SRC_DIR}/qwt_symbol.h
    ${QWT_SRC_DIR}/qwt_text_engine.h
    ${QWT_SRC_DIR}/qwt_text_label.h
    ${QWT_SRC_DIR}/qwt_text.h
    ${QWT_SRC_DIR}/qwt_valuelist.h
    #QWT_PLOT
    ${QWT_SRC_DIR}/qwt_curve_fitter.h
    ${QWT_SRC_DIR}/qwt_data.h
    ${QWT_SRC_DIR}/qwt_event_pattern.h
    ${QWT_SRC_DIR}/qwt_interval_data.h
    ${QWT_SRC_DIR}/qwt_legend.h
    ${QWT_SRC_DIR}/qwt_legend_item.h
    ${QWT_SRC_DIR}/qwt_legend_itemmanager.h
    ${QWT_SRC_DIR}/qwt_plot.h
    ${QWT_SRC_DIR}/qwt_plot_curve.h
    ${QWT_SRC_DIR}/qwt_plot_dict.h
    ${QWT_SRC_DIR}/qwt_plot_grid.h
    ${QWT_SRC_DIR}/qwt_plot_item.h
    ${QWT_SRC_DIR}/qwt_plot_layout.h
    ${QWT_SRC_DIR}/qwt_plot_marker.h
    ${QWT_SRC_DIR}/qwt_plot_printfilter.h
    ${QWT_SRC_DIR}/qwt_plot_rasteritem.h
    ${QWT_SRC_DIR}/qwt_plot_spectrogram.h
    ${QWT_SRC_DIR}/qwt_plot_scaleitem.h
    ${QWT_SRC_DIR}/qwt_plot_canvas.h
    ${QWT_SRC_DIR}/qwt_plot_rescaler.h
    ${QWT_SRC_DIR}/qwt_plot_panner.h
    ${QWT_SRC_DIR}/qwt_plot_picker.h
    ${QWT_SRC_DIR}/qwt_plot_zoomer.h
    ${QWT_SRC_DIR}/qwt_plot_magnifier.h
    ${QWT_SRC_DIR}/qwt_raster_data.h
    ${QWT_SRC_DIR}/qwt_scale_widget.h
)

#This will generate moc_* for Qt
QT4_WRAP_CPP(QWT_MOC_SRCS ${QWT_HEADERS})

#Include directories for QWT
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${QWT_SRC_DIR} plugins/ScopeView)

#Build library
add_library(qwt-wrap SHARED ${QWT_SRCS} ${QWT_MOC_SRCS} ${QWT_HEADERS})
target_link_libraries(qwt-wrap ${QT_LIBRARIES})
INSTALL(TARGETS qwt-wrap DESTINATION bin)
INSTALL(TARGETS qwt-wrap DESTINATION lib EXPORT qwt-wrap-export-targets)
INSTALL(EXPORT qwt-wrap-export-targets DESTINATION share/cmake)

INSTALL(FILES ${QWT_HEADERS} DESTINATION include/qwt)

SET(NetworkViewer_SRCS
	CANInterfaceHandler.cpp
	ModuleVariable.cpp
	ModuleConfiguration.cpp
	ModuleConfigurationView.cpp
	NetworkView.cpp
	NetworkModule.cpp
	NetworkModuleItem.cpp
	plugins/ScopeView/ScopeCurve.cpp
	plugins/ScopeView/ScopeView.cpp
	NetworkScheduler.cpp
	plugins/ScopeView/ScopeVariableConfig.cpp
	BasePlugin.cpp
	CANDevice.cpp
	DeviceSelectorDialog.cpp
        ModuleVariableTableWidget.cpp
        PreferencesDialog.cpp
)

SET(NetworkViewer_HEADERS 
	ModuleVariable.h
	ModuleConfiguration.h
	ModuleConfigurationView.h
	NetworkView.h
	NetworkModule.h
	NetworkModuleItem.h
	plugins/ScopeView/ScopeCurve.h
	plugins/ScopeView/ScopeView.h
	CANMessageEvent.h
	CANInterfaceHandler.h
	NetworkScheduler.h
	plugins/ScopeView/ScopeVariableConfig.h
	BasePlugin.h
	CAN_define.h
	CANDevice.h
	DeviceSelectorDialog.h
        NetworkViewerSplashScreen.h
        ModuleVariableTableWidget.h
        PreferencesDialog.h
)

SET(NetworkViewer_UIS
	ModuleConfigurationView.ui
	NetworkView.ui
	plugins/ScopeView/ScopeView.ui
	plugins/ScopeView/ScopeVariableConfig.ui
	TestView.ui
	DeviceSelectorDialog.ui
        PreferencesDialog.ui
)

SET(qrc 
	NetworkViewer.qrc
)



# generate rules for building source files from the resources
QT4_ADD_RESOURCES(SRCS_QRC ${qrc})

SET (QTVERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")

SET (XML_FILES
    config/INTROLAB_J0Pwr.xml
    config/INTROLAB_Unidrive_v2.xml
    config/INTROLAB_ReddyHead.xml
    config/INTROLAB_dsPICDrive.xml
)


#This will generate ui_* for Qt
QT4_WRAP_UI(NetworkViewer_UI_SRCS ${NetworkViewer_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(NetworkViewer_MOC_SRCS ${NetworkViewer_HEADERS})

#Include directories
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}
                ${PROJECT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}
                ${QWT_SRC_DIR}
                )


#Install headers
INSTALL( FILES ${NetworkViewer_HEADERS} DESTINATION include)
INSTALL( FILES ${NetworkViewer_UI_SRCS} DESTINATION include/ui)


#include files
IF (WIN32)



	SET (QT_DLL
			"${QT_BINARY_DIR}/Qt3Support4.dll"
			"${QT_BINARY_DIR}/QtCore4.dll"
			"${QT_BINARY_DIR}/QtDesigner4.dll"
			"${QT_BINARY_DIR}/QtDesignerComponents4.dll"
			"${QT_BINARY_DIR}/QtGui4.dll"
			"${QT_BINARY_DIR}/QtHelp4.dll"
			"${QT_BINARY_DIR}/QtNetwork4.dll"
			"${QT_BINARY_DIR}/QtOpenGL4.dll"
			"${QT_BINARY_DIR}/QtScript4.dll"
			"${QT_BINARY_DIR}/QtSql4.dll"
			"${QT_BINARY_DIR}/QtSvg4.dll"
			"${QT_BINARY_DIR}/QtTest4.dll"
			"${QT_BINARY_DIR}/QtWebKit4.dll"
			"${QT_BINARY_DIR}/QtXml4.dll"
                        "${QT_BINARY_DIR}/QtXmlPatterns4.dll"
                        "${QT_BINARY_DIR}/QtDeclarative4.dll"
                        "${QT_BINARY_DIR}/phonon4.dll"
			"${QT_BINARY_DIR}/mingwm10.dll"
			"${QT_BINARY_DIR}/libgcc_s_dw2-1.dll"
	    )

        #NetworkViewer Library
	add_library(networkViewerBase SHARED ${NetworkViewer_SRCS} ${NetworkViewer_MOC_SRCS} ${NetworkViewer_UI_SRCS} ${SRCS_QRC} ${NetworkViewer_HEADERS})
        target_link_libraries(networkViewerBase ${QT_LIBRARIES} qwt-wrap)
        INSTALL(TARGETS networkViewerBase DESTINATION bin)
	INSTALL(TARGETS networkViewerBase qwt-wrap DESTINATION lib EXPORT networkViewerBase-export-targets)
	INSTALL(EXPORT networkViewerBase-export-targets DESTINATION share/cmake)


        #NetworkViewer Executable
        add_executable(NetworkViewer WIN32 main.cpp)
	set_target_properties(NetworkViewer PROPERTIES LINK_FLAGS "--enable-auto-import")
	target_link_libraries(NetworkViewer networkViewerBase)
        INSTALL(TARGETS NetworkViewer DESTINATION bin)

        #Install DLLS
        INSTALL( FILES ${QT_DLL} DESTINATION bin)



        ###########################
        # Packaging stuff for WIN32
        ############################
        SET(CPACK_SOURCE_GENERATOR "ZIP")
        SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.txt")
        SET(CPACK_PACKAGE_NAME "OpenECoSys-NetworkViewer")
        SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NetworkViewer")
        SET(CPACK_PACKAGE_VENDOR "Dominic Letourneau")
        SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
        SET(CPACK_PACKAGE_CONTACT "dominic.letourneau@usherbrooke.ca")
        SET(CPACK_PACKAGE_INSTALL_DIRECTORY "NetworkViewer-${CPACK_PACKAGE_VERSION}")
        SET(CPACK_NSIS_URL_INFO_ABOUT "http://introlab.gel.usherbrooke.ca")
        SET(CPACK_NSIS_CONTACT "dominic.letourneau@usherbrooke.ca")
        SET(CPACK_NSIS_MODIFY_PATH OFF)
        SET(CPACK_PACKAGE_EXECUTABLES "NetworkViewer" "NetworkViewer")

        INCLUDE(CPack)
ENDIF (WIN32)

IF (APPLE)


        #NetworkViewer Library
	add_library(networkViewerBase SHARED ${NetworkViewer_SRCS} ${NetworkViewer_MOC_SRCS} ${NetworkViewer_UI_SRCS} ${SRCS_QRC} ${NetworkViewer_HEADERS})
        target_link_libraries(networkViewerBase ${QT_LIBRARIES} qwt-wrap)
	#set_target_properties(networkViewerBase PROPERTIES FRAMEWORK "true")
	INSTALL(TARGETS networkViewerBase qwt-wrap DESTINATION lib EXPORT networkViewerBase-export-targets)
	INSTALL(EXPORT networkViewerBase-export-targets DESTINATION share/cmake)


        #NetworkViewer Executable
        add_executable(NetworkViewer main.cpp)
	set_target_properties(NetworkViewer PROPERTIES LINK_FLAGS "--enable-auto-import")
	target_link_libraries(NetworkViewer networkViewerBase)
	INSTALL(TARGETS NetworkViewer DESTINATION bin)


        SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.txt")
        SET(CPACK_GENERATOR Bundle)

        #BUNDLE INFO
	SET(CPACK_BUNDLE_NAME "NetworkViewer")
        SET(MACOSX_BUNDLE_INFO_STRING "NetworkViewer")
	SET(CPACK_PACKAGE_DEFAULT_LOCATION "/Applications")
	SET(CPACK_BUNDLE_ICON "NetworkViewer.icns")
	SET(CPACK_PACKAGE_ICON "NetworkViewer.icns")
        SET(MACOSX_BUNDLE_GUI_IDENTIFIER "NetworkViewer")
        SET(MACOSX_BUNDLE_LONG_VERSION_STRING "NetworkViewer-${CPACK_PACKAGE_VERSION}")
        SET(MACOSX_BUNDLE_BUNDLE_NAME "NetworkViewer.app")
        SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
        SET(MACOSX_BUNDLE_BUNDLE_VERSION "${CPACK_PACKAGE_VERSION}")
        SET(MACOSX_BUNDLE_COPYRIGHT "README.txt")
        SET(CPACK_BUNDLE_PLIST "NetworkViewer.plist")
	SET(CPACK_BUNDLE_STARTUP_COMMAND "start_osx.sh")



        INCLUDE(CPack)

ENDIF (APPLE)

#TODO LINUX SPECIFIC...
IF (UNIX AND NOT APPLE)

    #NetworkViewer Library
    add_library(networkViewerBase SHARED ${NetworkViewer_SRCS} ${NetworkViewer_MOC_SRCS} ${NetworkViewer_UI_SRCS} ${SRCS_QRC} ${NetworkViewer_HEADERS})
    target_link_libraries(networkViewerBase ${QT_LIBRARIES} qwt-wrap)
    INSTALL(TARGETS networkViewerBase qwt-wrap DESTINATION lib EXPORT networkViewerBase-export-targets)
    INSTALL(EXPORT networkViewerBase-export-targets DESTINATION share/cmake)


    #NetworkViewer Executable
    add_executable(NetworkViewer main.cpp)
    set_target_properties(NetworkViewer PROPERTIES LINK_FLAGS "--enable-auto-import")
    target_link_libraries(NetworkViewer networkViewerBase)
    INSTALL(TARGETS NetworkViewer DESTINATION bin)

ENDIF(UNIX AND NOT APPLE)

ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(scripts)
ADD_SUBDIRECTORY(drivers)
ADD_SUBDIRECTORY(QextSerialPort)



#GENERIC INSTALL
INSTALL( FILES ${XML_FILES} DESTINATION config)


