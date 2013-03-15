#config

SET (XML_FILES
    INTROLAB_J0Pwr.xml
    INTROLAB_Unidrive_v1.xml
    INTROLAB_Unidrive_v2.xml
    INTROLAB_ReddyHead.xml
    INTROLAB_dsPICDrive.xml
	INTROLAB_Tactile.xml
    LoopbackTest.xml
    INTROLAB_ADELeg.xml
    NETVArduino_Analog.xml
)

#GENERIC INSTALL
INSTALL( FILES ${XML_FILES} DESTINATION config)
