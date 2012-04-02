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
# Adding MPLAB X bin directory to path
PATH:=/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/:$(PATH)
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=dsPIC30F4011
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ${OBJECTDIR}/eeprom_flash.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d ${OBJECTDIR}/eeprom_flash.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ${OBJECTDIR}/eeprom_flash.o ${OBJECTDIR}/main.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="/System/Library/Java/JavaVirtualMachines/1.6.0.jdk/Contents/Home/bin/"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC="/Applications/microchip/mplabc30/v3.30b/bin/pic30-gcc"
# MP_BC is not defined
MP_AS="/Applications/microchip/mplabc30/v3.30b/bin/pic30-as"
MP_LD="/Applications/microchip/mplabc30/v3.30b/bin/pic30-ld"
MP_AR="/Applications/microchip/mplabc30/v3.30b/bin/pic30-ar"
DEP_GEN=${MP_JAVA_PATH}java -jar "/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/extractobjectdependencies.jar" 
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps
MP_CC_DIR="/Applications/microchip/mplabc30/v3.30b/bin"
# MP_BC_DIR is not defined
MP_AS_DIR="/Applications/microchip/mplabc30/v3.30b/bin"
MP_LD_DIR="/Applications/microchip/mplabc30/v3.30b/bin"
MP_AR_DIR="/Applications/microchip/mplabc30/v3.30b/bin"
# MP_BC_DIR is not defined

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-dsPIC30F4011.mk dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F4011
MP_LINKER_FILE_OPTION=,--script="p30F4011_bootloader.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/eeprom_flash.o: eeprom_flash.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.d 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.ok ${OBJECTDIR}/eeprom_flash.o.err 
	@${FIXDEPS} "${OBJECTDIR}/eeprom_flash.o.d" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE)  eeprom_flash.s -o ${OBJECTDIR}/eeprom_flash.o -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 --defsym=__MPLAB_DEBUG=1 --defsym=__ICD2RAM=1 --defsym=__DEBUG=1 --defsym=__MPLAB_DEBUGGER_ICD3=1 -g  -MD "${OBJECTDIR}/eeprom_flash.o.d"$(MP_EXTRA_AS_POST)
	
else
${OBJECTDIR}/eeprom_flash.o: eeprom_flash.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.d 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.ok ${OBJECTDIR}/eeprom_flash.o.err 
	@${FIXDEPS} "${OBJECTDIR}/eeprom_flash.o.d" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE)  eeprom_flash.s -o ${OBJECTDIR}/eeprom_flash.o -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 -g  -MD "${OBJECTDIR}/eeprom_flash.o.d"$(MP_EXTRA_AS_POST)
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o: ../NETVProtocolStack/NETV16/NETV16_CANDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1908611077 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F4011_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF "${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d" -o ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ../NETVProtocolStack/NETV16/NETV16_CANDriver.c  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.ok ${OBJECTDIR}/main.o.err 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F4011_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  
	
else
${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o: ../NETVProtocolStack/NETV16/NETV16_CANDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1908611077 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F4011_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF "${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d" -o ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ../NETVProtocolStack/NETV16/NETV16_CANDriver.c  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.ok ${OBJECTDIR}/main.o.err 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F4011_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}/pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=elf
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/dsPIC30F4011
	${RM} -r dist/dsPIC30F4011

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
