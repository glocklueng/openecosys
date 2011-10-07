#console/src



SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTXML TRUE)


include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/core/src)

#NetworkViewer Executable
add_executable(netvconsole main.cpp)
set_target_properties(netvconsole PROPERTIES LINK_FLAGS "--enable-auto-import")
target_link_libraries(netvconsole networkViewerCore ${QT_LIBRARIES})
INSTALL(TARGETS netvconsole DESTINATION bin)
