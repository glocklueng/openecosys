########################
# Configurations
########################

SET (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/drivers)

ADD_SUBDIRECTORY(SerialBridge)
ADD_SUBDIRECTORY(EthernetBridge)

IF (NOT APPLE)
	ADD_SUBDIRECTORY(PCAN)
ENDIF (NOT APPLE)


