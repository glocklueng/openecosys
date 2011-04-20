#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
MKDIR=mkdir -p
RM=rm -f 
CP=cp 
# Macros
CND_CONF=default

ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic32dev_beta.X.${IMAGE_TYPE}.elf
else
IMAGE_TYPE=production
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic32dev_beta.X.${IMAGE_TYPE}.elf
endif
# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}
# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/65181253/usb_function_cdc.o ${OBJECTDIR}/_ext/1472/NETV32_Memory.o ${OBJECTDIR}/serial.o ${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o ${OBJECTDIR}/pic32mx_cfg.o ${OBJECTDIR}/_ext/1472/NETV32_Common.o ${OBJECTDIR}/_ext/1472/NETV32_Shared.o ${OBJECTDIR}/_ext/65181253/usb_descriptors.o ${OBJECTDIR}/_ext/65181253/usb_device.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

OS_ORIGINAL="Darwin"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC=/Applications/microchip/mplabc32/v1.11a/bin/pic32-gcc
# MP_BC is not defined
MP_AS=/Applications/microchip/mplabc32/v1.11a/bin/pic32-as
MP_LD=/Applications/microchip/mplabc32/v1.11a/bin/pic32-ld
MP_AR=/Applications/microchip/mplabc32/v1.11a/bin/pic32-ar
# MP_BC is not defined
MP_CC_DIR=/Applications/microchip/mplabc32/v1.11a/bin
# MP_BC_DIR is not defined
MP_AS_DIR=/Applications/microchip/mplabc32/v1.11a/bin
MP_LD_DIR=/Applications/microchip/mplabc32/v1.11a/bin
MP_AR_DIR=/Applications/microchip/mplabc32/v1.11a/bin
# MP_BC_DIR is not defined
.build-conf: ${BUILD_SUBPROJECTS}
ifneq ($(OS_CURRENT),$(OS_ORIGINAL))
	@echo "***** WARNING: This make file contains OS dependent code. The OS this makefile is being run is different from the OS it was created in."
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/pic32dev_beta.X.${IMAGE_TYPE}.elf

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/65181253/usb_function_cdc.o: ../USB-CDC_Microchip/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/65181253 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d -o ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o ../USB-CDC_Microchip/usb_function_cdc.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d > ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.tmp
	${RM} ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d 
	${CP} ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.tmp ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.tmp}
endif
${OBJECTDIR}/_ext/1472/NETV32_Memory.o: ../NETV32_Memory.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d -o ${OBJECTDIR}/_ext/1472/NETV32_Memory.o ../NETV32_Memory.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d > ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.tmp ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.tmp}
endif
${OBJECTDIR}/serial.o: serial.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
	${RM} ${OBJECTDIR}/serial.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/serial.o.d -o ${OBJECTDIR}/serial.o serial.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/serial.o.d > ${OBJECTDIR}/serial.o.tmp
	${RM} ${OBJECTDIR}/serial.o.d 
	${CP} ${OBJECTDIR}/serial.o.tmp ${OBJECTDIR}/serial.o.d 
	${RM} ${OBJECTDIR}/serial.o.tmp}
endif
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
	${RM} ${OBJECTDIR}/main.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/main.o.d -o ${OBJECTDIR}/main.o main.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/main.o.d > ${OBJECTDIR}/main.o.tmp
	${RM} ${OBJECTDIR}/main.o.d 
	${CP} ${OBJECTDIR}/main.o.tmp ${OBJECTDIR}/main.o.d 
	${RM} ${OBJECTDIR}/main.o.tmp}
endif
${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o: ../NETV32_SerialDriver.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d -o ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o ../NETV32_SerialDriver.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d > ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.tmp ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.tmp}
endif
${OBJECTDIR}/pic32mx_cfg.o: pic32mx_cfg.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
	${RM} ${OBJECTDIR}/pic32mx_cfg.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/pic32mx_cfg.o.d -o ${OBJECTDIR}/pic32mx_cfg.o pic32mx_cfg.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/pic32mx_cfg.o.d > ${OBJECTDIR}/pic32mx_cfg.o.tmp
	${RM} ${OBJECTDIR}/pic32mx_cfg.o.d 
	${CP} ${OBJECTDIR}/pic32mx_cfg.o.tmp ${OBJECTDIR}/pic32mx_cfg.o.d 
	${RM} ${OBJECTDIR}/pic32mx_cfg.o.tmp}
endif
${OBJECTDIR}/_ext/1472/NETV32_Common.o: ../NETV32_Common.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d -o ${OBJECTDIR}/_ext/1472/NETV32_Common.o ../NETV32_Common.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d > ${OBJECTDIR}/_ext/1472/NETV32_Common.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/NETV32_Common.o.tmp ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Common.o.tmp}
endif
${OBJECTDIR}/_ext/1472/NETV32_Shared.o: ../NETV32_Shared.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d -o ${OBJECTDIR}/_ext/1472/NETV32_Shared.o ../NETV32_Shared.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d > ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.tmp ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.tmp}
endif
${OBJECTDIR}/_ext/65181253/usb_descriptors.o: ../USB-CDC_Microchip/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/65181253 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d -o ${OBJECTDIR}/_ext/65181253/usb_descriptors.o ../USB-CDC_Microchip/usb_descriptors.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d > ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.tmp
	${RM} ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d 
	${CP} ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.tmp ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.tmp}
endif
${OBJECTDIR}/_ext/65181253/usb_device.o: ../USB-CDC_Microchip/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/65181253 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_device.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/65181253/usb_device.o.d -o ${OBJECTDIR}/_ext/65181253/usb_device.o ../USB-CDC_Microchip/usb_device.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/65181253/usb_device.o.d > ${OBJECTDIR}/_ext/65181253/usb_device.o.tmp
	${RM} ${OBJECTDIR}/_ext/65181253/usb_device.o.d 
	${CP} ${OBJECTDIR}/_ext/65181253/usb_device.o.tmp ${OBJECTDIR}/_ext/65181253/usb_device.o.d 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_device.o.tmp}
endif
else
${OBJECTDIR}/_ext/65181253/usb_function_cdc.o: ../USB-CDC_Microchip/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/65181253 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d -o ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o ../USB-CDC_Microchip/usb_function_cdc.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d > ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.tmp
	${RM} ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d 
	${CP} ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.tmp ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.d 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_function_cdc.o.tmp}
endif
${OBJECTDIR}/_ext/1472/NETV32_Memory.o: ../NETV32_Memory.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d -o ${OBJECTDIR}/_ext/1472/NETV32_Memory.o ../NETV32_Memory.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d > ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.tmp ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Memory.o.tmp}
endif
${OBJECTDIR}/serial.o: serial.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
	${RM} ${OBJECTDIR}/serial.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/serial.o.d -o ${OBJECTDIR}/serial.o serial.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/serial.o.d > ${OBJECTDIR}/serial.o.tmp
	${RM} ${OBJECTDIR}/serial.o.d 
	${CP} ${OBJECTDIR}/serial.o.tmp ${OBJECTDIR}/serial.o.d 
	${RM} ${OBJECTDIR}/serial.o.tmp}
endif
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
	${RM} ${OBJECTDIR}/main.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/main.o.d -o ${OBJECTDIR}/main.o main.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/main.o.d > ${OBJECTDIR}/main.o.tmp
	${RM} ${OBJECTDIR}/main.o.d 
	${CP} ${OBJECTDIR}/main.o.tmp ${OBJECTDIR}/main.o.d 
	${RM} ${OBJECTDIR}/main.o.tmp}
endif
${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o: ../NETV32_SerialDriver.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d -o ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o ../NETV32_SerialDriver.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d > ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.tmp ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_SerialDriver.o.tmp}
endif
${OBJECTDIR}/pic32mx_cfg.o: pic32mx_cfg.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
	${RM} ${OBJECTDIR}/pic32mx_cfg.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/pic32mx_cfg.o.d -o ${OBJECTDIR}/pic32mx_cfg.o pic32mx_cfg.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/pic32mx_cfg.o.d > ${OBJECTDIR}/pic32mx_cfg.o.tmp
	${RM} ${OBJECTDIR}/pic32mx_cfg.o.d 
	${CP} ${OBJECTDIR}/pic32mx_cfg.o.tmp ${OBJECTDIR}/pic32mx_cfg.o.d 
	${RM} ${OBJECTDIR}/pic32mx_cfg.o.tmp}
endif
${OBJECTDIR}/_ext/1472/NETV32_Common.o: ../NETV32_Common.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d -o ${OBJECTDIR}/_ext/1472/NETV32_Common.o ../NETV32_Common.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d > ${OBJECTDIR}/_ext/1472/NETV32_Common.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/NETV32_Common.o.tmp ${OBJECTDIR}/_ext/1472/NETV32_Common.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Common.o.tmp}
endif
${OBJECTDIR}/_ext/1472/NETV32_Shared.o: ../NETV32_Shared.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d -o ${OBJECTDIR}/_ext/1472/NETV32_Shared.o ../NETV32_Shared.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d > ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.tmp ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/NETV32_Shared.o.tmp}
endif
${OBJECTDIR}/_ext/65181253/usb_descriptors.o: ../USB-CDC_Microchip/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/65181253 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d -o ${OBJECTDIR}/_ext/65181253/usb_descriptors.o ../USB-CDC_Microchip/usb_descriptors.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d > ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.tmp
	${RM} ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d 
	${CP} ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.tmp ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.d 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_descriptors.o.tmp}
endif
${OBJECTDIR}/_ext/65181253/usb_device.o: ../USB-CDC_Microchip/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/65181253 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_device.o.d 
	${MP_CC}  -x c -c -mprocessor=32MX795F512H -I"./" -I"../" -I"../USB-CDC_Microchip" -MMD -MF ${OBJECTDIR}/_ext/65181253/usb_device.o.d -o ${OBJECTDIR}/_ext/65181253/usb_device.o ../USB-CDC_Microchip/usb_device.c 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	 sed -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/65181253/usb_device.o.d > ${OBJECTDIR}/_ext/65181253/usb_device.o.tmp
	${RM} ${OBJECTDIR}/_ext/65181253/usb_device.o.d 
	${CP} ${OBJECTDIR}/_ext/65181253/usb_device.o.tmp ${OBJECTDIR}/_ext/65181253/usb_device.o.d 
	${RM} ${OBJECTDIR}/_ext/65181253/usb_device.o.tmp}
endif
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/pic32dev_beta.X.${IMAGE_TYPE}.elf: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=32MX795F512H  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic32dev_beta.X.${IMAGE_TYPE}.elf ${OBJECTFILES}      -Wl,--defsym=__MPLAB_BUILD=1,--report-mem,--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1 
else
dist/${CND_CONF}/${IMAGE_TYPE}/pic32dev_beta.X.${IMAGE_TYPE}.elf: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  -mprocessor=32MX795F512H  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic32dev_beta.X.${IMAGE_TYPE}.elf ${OBJECTFILES}      -Wl,--defsym=__MPLAB_BUILD=1,--report-mem
	${MP_CC_DIR}/pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/pic32dev_beta.X.${IMAGE_TYPE}.elf  
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
