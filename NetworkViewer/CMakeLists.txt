PROJECT (OpenEcoSys-NetworkViewer)

cmake_minimum_required(VERSION 2.6)


########################
# Configurations
########################

SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "4")
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})



#OSX Architecture, will build for 32 bits systems
#set(CMAKE_OSX_ARCHITECTURES "i386")




SET(CMAKE_BUILD_TYPE Release)

IF(WIN32)
	MESSAGE( STATUS "Windows version: " ${CMAKE_SYSTEM})
	SET(CMAKE_INSTALL_PREFIX "$ENV{SYSTEMDRIVE}/NetworkViewer-${CMAKE_BUILD_TYPE}")
	MESSAGE( STATUS "Install directory is : " ${CMAKE_INSTALL_PREFIX})
ELSE(WIN32)
	SET(CMAKE_INSTALL_PREFIX "/usr/local/NetworkViewer")
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

include(${QT_USE_FILE})



SET(NetworkViewer_SRCS
	CANInterfaceHandler.cpp
	ModuleVariable.cpp
	ModuleConfiguration.cpp
	ModuleConfigurationView.cpp
	NetworkView.cpp
	NetworkModule.cpp
	NetworkModuleItem.cpp
	ScopeCurve.cpp
	ScopeView.cpp
	NetworkScheduler.cpp
	ScopeVariableConfig.cpp
	BasePlugin.cpp
	CANDevice.cpp
	DeviceSelectorDialog.cpp
        ModuleVariableTableWidget.cpp
)

SET(NetworkViewer_HEADERS 
	ModuleVariable.h
	ModuleConfiguration.h
	ModuleConfigurationView.h
	NetworkView.h
	NetworkModule.h
	NetworkModuleItem.h
	ScopeCurve.h
	ScopeView.h
	CANMessageEvent.h
	CANInterfaceHandler.h
	NetworkScheduler.h
	ScopeVariableConfig.h
	BasePlugin.h
	CAN_define.h
	CANDevice.h
	DeviceSelectorDialog.h
        NetworkViewerSplashScreen.h
        ModuleVariableTableWidget.h
)

SET(NetworkViewer_UIS
	ModuleConfigurationView.ui
	NetworkView.ui
	ScopeView.ui
	ScopeVariableConfig.ui
	TestView.ui
	DeviceSelectorDialog.ui
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
                ${PROJECT_SOURCE_DIR}/
                ${CMAKE_CURRENT_BINARY_DIR}
                )



#include files
IF (WIN32)

        #QWT INCLUDE
        INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/qwt/WIN32/include )


	SET (QT_DLL
			"${QT_BINARY_DIR}/Qt3Support4.dll"
			"${QT_BINARY_DIR}/QtAssistantClient4.dll"
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
                        "${QT_BINARY_DIR}/phonon4.dll"
			"./qwt/WIN32/lib/qwt5.dll"
			"${QT_BINARY_DIR}/mingwm10.dll"
			"${QT_BINARY_DIR}/libgcc_s_dw2-1.dll"
	    )

        #NetworkViewer Library
	add_library(networkViewerBase SHARED ${NetworkViewer_SRCS} ${NetworkViewer_MOC_SRCS} ${NetworkViewer_UI_SRCS} ${SRCS_QRC} ${NetworkViewer_HEADERS})
	target_link_libraries(networkViewerBase ${QT_LIBRARIES} -L${PROJECT_SOURCE_DIR}/qwt/WIN32/lib -lqwt5 )
        INSTALL(TARGETS networkViewerBase DESTINATION bin)


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
        SET(CPACK_PACKAGE_NAME "OpenEcoSys-NetworkViewer")
        SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NetworkViewer")
        SET(CPACK_PACKAGE_VENDOR "Dominic Letourneau")
        SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
        SET(CPACK_PACKAGE_CONTACT "dominic.letourneau@usherbrooke.ca")
        SET(CPACK_PACKAGE_INSTALL_DIRECTORY "NetworkViewer")
        SET(CPACK_NSIS_URL_INFO_ABOUT "http://introlab.gel.usherbrooke.ca")
        SET(CPACK_NSIS_CONTACT "dominic.letourneau@usherbrooke.ca")
        SET(CPACK_NSIS_MODIFY_PATH ON)
        SET(CPACK_PACKAGE_EXECUTABLES "NetworkViewer" "NetworkViewer")

        INCLUDE(CPack)
ENDIF (WIN32)

IF (APPLE)
        INCLUDE_DIRECTORIES({QT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/qwt/OSX_x64/include)

        #NetworkViewer Library
	add_library(networkViewerBase SHARED ${NetworkViewer_SRCS} ${NetworkViewer_MOC_SRCS} ${NetworkViewer_UI_SRCS} ${SRCS_QRC} ${NetworkViewer_HEADERS})
        target_link_libraries(networkViewerBase ${QT_LIBRARIES} -L${PROJECT_SOURCE_DIR}/qwt/OSX_x64/lib -lqwt )
        INSTALL(TARGETS networkViewerBase DESTINATION lib)


        #NetworkViewer Executable
        add_executable(NetworkViewer main.cpp)
	set_target_properties(NetworkViewer PROPERTIES LINK_FLAGS "--enable-auto-import")
	target_link_libraries(NetworkViewer networkViewerBase)
	INSTALL(TARGETS NetworkViewer DESTINATION bin)

ENDIF (APPLE)

#TODO LINUX SPECIFIC...
IF (UNIX AND NOT APPLE)

    INCLUDE_DIRECTORIES({QT_INCLUDE_DIR} ./ /usr/include/qwt-qt4 /usr/include)

    #NetworkViewer Library
    add_library(networkViewerBase SHARED ${NetworkViewer_SRCS} ${NetworkViewer_MOC_SRCS} ${NetworkViewer_UI_SRCS} ${SRCS_QRC} ${NetworkViewer_HEADERS})
    target_link_libraries(networkViewerBase ${QT_LIBRARIES} -L/usr/lib -lqwt-qt4 )
    INSTALL(TARGETS networkViewerBase DESTINATION lib)


    #NetworkViewer Executable
    add_executable(NetworkViewer main.cpp)
    set_target_properties(NetworkViewer PROPERTIES LINK_FLAGS "--enable-auto-import")
    target_link_libraries(NetworkViewer networkViewerBase)
    INSTALL(TARGETS NetworkViewer DESTINATION bin)

ENDIF(UNIX AND NOT APPLE)

ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(scripts)
ADD_SUBDIRECTORY(drivers)


#GENERIC INSTALL
INSTALL( FILES ${XML_FILES} DESTINATION config)


