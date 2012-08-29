MESSAGE( STATUS "Buiding PCAN driver...")



IF (WIN32)
	SET(PCAN_SRCS PCANDevice.cpp)
ELSE (WIN32)
	SET(PCAN_SRCS PCANDeviceLinux.cpp)
ENDIF(WIN32)

SET(PCAN_HEADERS
        PCANDevice.h
)

SET(PCAN_UIS

)

SET(PCAN_QRC

)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})


# generate rules for building source files from the resources
QT4_ADD_RESOURCES(PCAN_QRC_SRCS ${PCAN_QRC})

#This will generate ui_* for Qt
QT4_WRAP_UI(PCAN_UI_SRCS ${PCAN_UIS})

#This will generate moc_* for Qt
QT4_WRAP_CPP(PCAN_MOC_SRCS ${PCAN_HEADERS})

INCLUDE_DIRECTORIES(${DRIVER_INCLUDES} ${PROJECT_SOURCE_DIR}/core/drivers/PCAN ${CMAKE_CURRENT_BINARY_DIR})


IF(WIN32)

	MESSAGE( STATUS "Buiding PCAN driver for Windows...")

	add_library(PCANDriver SHARED ${PCAN_SRCS} ${PCAN_HEADERS} ${PCAN_QRC_SRCS} ${PCAN_MOC_SRCS} ${PCAN_UI_SRCS}  )
        target_link_libraries(PCANDriver  ${DRIVER_LIBRARIES} -L${PROJECT_SOURCE_DIR}/core/drivers/PCAN PCANBasic.dll)
	set_target_properties(PCANDriver PROPERTIES LINK_FLAGS "--enable-auto-import")
	INSTALL(TARGETS PCANDriver DESTINATION drivers/PCANDriver)

	SET(PCAN_DLL
                PCANBasic.dll
	)

	#Install Windows DLLS
	INSTALL( FILES ${PCAN_DLL} DESTINATION bin)

ENDIF(WIN32)


IF (UNIX AND NOT APPLE)
        FIND_FILE(PCAN_FILE "/usr/include/pwd.h")
        MESSAGE( STATUS "Buiding PCAN driver for Linux... ${PCAN_FILE}")
	add_library(PCANDriver SHARED ${PCAN_SRCS} ${PCAN_HEADERS} ${PCAN_QRC_SRCS} ${PCAN_MOC_SRCS} ${PCAN_UI_SRCS}  )
        target_link_libraries(PCANDriver  ${DRIVER_LIBRARIES} -L/usr/lib -lpcan)
	INSTALL(TARGETS PCANDriver DESTINATION drivers/PCANDriver)

ENDIF(UNIX AND NOT APPLE)


