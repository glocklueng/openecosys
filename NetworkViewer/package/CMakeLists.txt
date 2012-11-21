#package

########################
# Configurations
########################
SET(CPACK_PACKAGE_VERSION_MAJOR ${NETWORKVIEWER_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${NETWORKVIEWER_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${NETWORKVIEWER_VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})


IF (WIN32)


        #Installing Qt libraries...


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
                        "C:/QtSDK/mingw/bin/mingwm10.dll"
                        "C:/QtSDK/mingw/bin/libgcc_s_dw2-1.dll"
	    )

        #Install DLLS
        INSTALL( FILES ${QT_DLL} DESTINATION bin)

        ###########################
        # Packaging stuff for WIN32
        ############################
        SET(CPACK_SOURCE_GENERATOR "ZIP")
        SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.txt")
        SET(CPACK_PACKAGE_NAME "OpenECoSys-NetworkViewer")
        SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NetworkViewer")
        SET(CPACK_PACKAGE_VENDOR "Dominic Letourneau")
        SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.txt")
        SET(CPACK_PACKAGE_CONTACT "dominic.letourneau@usherbrooke.ca")
        SET(CPACK_PACKAGE_INSTALL_DIRECTORY "NetworkViewer-${CPACK_PACKAGE_VERSION}")
        SET(CPACK_NSIS_URL_INFO_ABOUT "http://introlab.gel.usherbrooke.ca")
        SET(CPACK_NSIS_CONTACT "dominic.letourneau@usherbrooke.ca")
        SET(CPACK_NSIS_MODIFY_PATH OFF)
        SET(CPACK_PACKAGE_EXECUTABLES "NetworkViewer" "NetworkViewer")

        INCLUDE(CPack)
ENDIF (WIN32)

IF (APPLE)

	SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.txt")
	SET(CPACK_GENERATOR Bundle)

	#BUNDLE INFO
	SET(CPACK_BUNDLE_NAME "NetworkViewer")
	SET(MACOSX_BUNDLE_INFO_STRING "NetworkViewer")
	SET(CPACK_PACKAGE_DEFAULT_LOCATION "/Applications")
	SET(CPACK_BUNDLE_ICON "${PROJECT_SOURCE_DIR}/package/NetworkViewer.icns")
	SET(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/package/NetworkViewer.icns")
	SET(MACOSX_BUNDLE_GUI_IDENTIFIER "NetworkViewer")
	SET(MACOSX_BUNDLE_LONG_VERSION_STRING "NetworkViewer-${CPACK_PACKAGE_VERSION}")
	SET(MACOSX_BUNDLE_BUNDLE_NAME "NetworkViewer.app")
	SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
	SET(MACOSX_BUNDLE_BUNDLE_VERSION "${CPACK_PACKAGE_VERSION}")
        SET(MACOSX_BUNDLE_COPYRIGHT "${PROJECT_SOURCE_DIR}/README.txt")
	SET(CPACK_BUNDLE_PLIST "${PROJECT_SOURCE_DIR}/package/NetworkViewer.plist")
	SET(CPACK_BUNDLE_STARTUP_COMMAND "${PROJECT_SOURCE_DIR}/package/start_osx.sh")

	INCLUDE(CPack)

ENDIF (APPLE)

#TODO LINUX SPECIFIC...
IF (UNIX AND NOT APPLE)

ENDIF(UNIX AND NOT APPLE)
