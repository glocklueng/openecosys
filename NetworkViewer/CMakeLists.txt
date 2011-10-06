PROJECT (OpenECoSys-NetworkViewer)

cmake_minimum_required(VERSION 2.8)

########################
# Configurations
########################

SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "16alpha")
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})


#OSX Architecture, will build for 32 bits systems
#set(CMAKE_OSX_ARCHITECTURES "i386")

ADD_DEFINITIONS(-DNETWORKVIEWER_VERSION="${CPACK_PACKAGE_VERSION}")

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




SET (XML_FILES
    config/INTROLAB_J0Pwr.xml
    config/INTROLAB_Unidrive_v1.xml
    config/INTROLAB_Unidrive_v2.xml
    config/INTROLAB_ReddyHead.xml
    config/INTROLAB_dsPICDrive.xml
    config/LoopbackTest.xml
    config/INTROLAB_ADELeg.xml
    config/NETVArduino_Analog.xml
)





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
        target_link_libraries(networkViewerBase ${QT_LIBRARIES})
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
    target_link_libraries(networkViewerBase ${QT_LIBRARIES})
    INSTALL(TARGETS networkViewerBase qwt-wrap DESTINATION lib EXPORT networkViewerBase-export-targets)
    INSTALL(EXPORT networkViewerBase-export-targets DESTINATION share/cmake)


    #NetworkViewer Executable
    add_executable(NetworkViewer WIN32 main.cpp)
    set_target_properties(NetworkViewer PROPERTIES LINK_FLAGS "--enable-auto-import")
    target_link_libraries(NetworkViewer networkViewerBase)
    INSTALL(TARGETS NetworkViewer DESTINATION bin)

ENDIF(UNIX AND NOT APPLE)

ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(scripts)
ADD_SUBDIRECTORY(drivers)
ADD_SUBDIRECTORY(QextSerialPort)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(console)
ADD_SUBDIRECTORY(GUI)


#GENERIC INSTALL
INSTALL( FILES ${XML_FILES} DESTINATION config)


