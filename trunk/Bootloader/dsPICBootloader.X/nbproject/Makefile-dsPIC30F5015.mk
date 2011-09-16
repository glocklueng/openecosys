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
CND_CONF=dsPIC30F5015
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.elf
else
IMAGE_TYPE=production
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.elf
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ${OBJECTDIR}/eeprom_flash.o ${OBJECTDIR}/main.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH=/System/Library/Java/JavaVirtualMachines/1.6.0.jdk/Contents/Home/bin/
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC=/Applications/microchip/mplabc30/v3.30b/bin/pic30-gcc
# MP_BC is not defined
MP_AS=/Applications/microchip/mplabc30/v3.30b/bin/pic30-as
MP_LD=/Applications/microchip/mplabc30/v3.30b/bin/pic30-ld
MP_AR=/Applications/microchip/mplabc30/v3.30b/bin/pic30-ar
# MP_BC is not defined
MP_CC_DIR=/Applications/microchip/mplabc30/v3.30b/bin
# MP_BC_DIR is not defined
MP_AS_DIR=/Applications/microchip/mplabc30/v3.30b/bin
MP_LD_DIR=/Applications/microchip/mplabc30/v3.30b/bin
MP_AR_DIR=/Applications/microchip/mplabc30/v3.30b/bin
# MP_BC_DIR is not defined

.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-dsPIC30F5015.mk dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.elf

MP_PROCESSOR_OPTION=30F5015
MP_LINKER_FILE_OPTION=,--script=p30F4011_bootloader.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/eeprom_flash.o: eeprom_flash.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.d 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.ok ${OBJECTDIR}/eeprom_flash.o.err 
	@echo ${MP_AS} $(MP_EXTRA_AS_PRE)  eeprom_flash.s -o ${OBJECTDIR}/eeprom_flash.o -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 --defsym=__MPLAB_DEBUG=1 --defsym=__ICD2RAM=1 --defsym=__DEBUG=1 --defsym=__MPLAB_DEBUGGER_ICD3=1 -g  -MD ${OBJECTDIR}/eeprom_flash.o.d$(MP_EXTRA_AS_POST)
	@-${MP_AS} $(MP_EXTRA_AS_PRE)  eeprom_flash.s -o ${OBJECTDIR}/eeprom_flash.o -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 --defsym=__MPLAB_DEBUG=1 --defsym=__ICD2RAM=1 --defsym=__DEBUG=1 --defsym=__MPLAB_DEBUGGER_ICD3=1 -g  -MD ${OBJECTDIR}/eeprom_flash.o.d$(MP_EXTRA_AS_POST) 2>&1 > ${OBJECTDIR}/eeprom_flash.o.err  ; if [ $$? -eq 0 ] ; then touch ${OBJECTDIR}/eeprom_flash.o.ok ; fi 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@if ! [ -f ${OBJECTDIR}/eeprom_flash.o.d ] ; then touch ${OBJECTDIR}/eeprom_flash.o.d ; fi 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/eeprom_flash.o.d > ${OBJECTDIR}/eeprom_flash.o.tmp
	@${RM} ${OBJECTDIR}/eeprom_flash.o.d 
	@${CP} ${OBJECTDIR}/eeprom_flash.o.tmp ${OBJECTDIR}/eeprom_flash.o.d 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.tmp}
else 
	@if ! [ -f ${OBJECTDIR}/eeprom_flash.o.d ] ; then touch ${OBJECTDIR}/eeprom_flash.o.d ; fi 
	@sed -e 's/\"//g' ${OBJECTDIR}/eeprom_flash.o.d > ${OBJECTDIR}/eeprom_flash.o.tmp
	@${RM} ${OBJECTDIR}/eeprom_flash.o.d 
	@${CP} ${OBJECTDIR}/eeprom_flash.o.tmp ${OBJECTDIR}/eeprom_flash.o.d 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.tmp
endif
	@touch ${OBJECTDIR}/eeprom_flash.o.err 
	@cat ${OBJECTDIR}/eeprom_flash.o.err 
	@if [ -f ${OBJECTDIR}/eeprom_flash.o.ok ] ; then rm -f ${OBJECTDIR}/eeprom_flash.o.ok; else exit 1; fi
	
else
${OBJECTDIR}/eeprom_flash.o: eeprom_flash.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.d 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.ok ${OBJECTDIR}/eeprom_flash.o.err 
	@echo ${MP_AS} $(MP_EXTRA_AS_PRE)  eeprom_flash.s -o ${OBJECTDIR}/eeprom_flash.o -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1  -MD ${OBJECTDIR}/eeprom_flash.o.d$(MP_EXTRA_AS_POST)
	@-${MP_AS} $(MP_EXTRA_AS_PRE)  eeprom_flash.s -o ${OBJECTDIR}/eeprom_flash.o -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1  -MD ${OBJECTDIR}/eeprom_flash.o.d$(MP_EXTRA_AS_POST) 2>&1 > ${OBJECTDIR}/eeprom_flash.o.err  ; if [ $$? -eq 0 ] ; then touch ${OBJECTDIR}/eeprom_flash.o.ok ; fi 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@if ! [ -f ${OBJECTDIR}/eeprom_flash.o.d ] ; then touch ${OBJECTDIR}/eeprom_flash.o.d ; fi 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/eeprom_flash.o.d > ${OBJECTDIR}/eeprom_flash.o.tmp
	@${RM} ${OBJECTDIR}/eeprom_flash.o.d 
	@${CP} ${OBJECTDIR}/eeprom_flash.o.tmp ${OBJECTDIR}/eeprom_flash.o.d 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.tmp}
else 
	@if ! [ -f ${OBJECTDIR}/eeprom_flash.o.d ] ; then touch ${OBJECTDIR}/eeprom_flash.o.d ; fi 
	@sed -e 's/\"//g' ${OBJECTDIR}/eeprom_flash.o.d > ${OBJECTDIR}/eeprom_flash.o.tmp
	@${RM} ${OBJECTDIR}/eeprom_flash.o.d 
	@${CP} ${OBJECTDIR}/eeprom_flash.o.tmp ${OBJECTDIR}/eeprom_flash.o.d 
	@${RM} ${OBJECTDIR}/eeprom_flash.o.tmp
endif
	@touch ${OBJECTDIR}/eeprom_flash.o.err 
	@cat ${OBJECTDIR}/eeprom_flash.o.err 
	@if [ -f ${OBJECTDIR}/eeprom_flash.o.ok ] ; then rm -f ${OBJECTDIR}/eeprom_flash.o.ok; else exit 1; fi
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.ok ${OBJECTDIR}/main.o.err 
	@echo ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F5015_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF ${OBJECTDIR}/main.o.d -o ${OBJECTDIR}/main.o main.c  
	@-${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F5015_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF ${OBJECTDIR}/main.o.d -o ${OBJECTDIR}/main.o main.c    2>&1  > ${OBJECTDIR}/main.o.err ; if [ $$? -eq 0 ] ; then touch ${OBJECTDIR}/main.o.ok ; fi 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/main.o.d > ${OBJECTDIR}/main.o.tmp
	@${RM} ${OBJECTDIR}/main.o.d 
	@${CP} ${OBJECTDIR}/main.o.tmp ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.tmp 
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/main.o.d > ${OBJECTDIR}/main.o.tmp
	@${RM} ${OBJECTDIR}/main.o.d 
	@${CP} ${OBJECTDIR}/main.o.tmp ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.tmp
endif
	@touch ${OBJECTDIR}/main.o.err 
	@cat ${OBJECTDIR}/main.o.err 
	@if [ -f ${OBJECTDIR}/main.o.ok ] ; then rm -f ${OBJECTDIR}/main.o.ok; else exit 1; fi
	
${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o: ../NETVProtocolStack/NETV16/NETV16_CANDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1908611077 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err 
	@echo ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F5015_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d -o ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ../NETVProtocolStack/NETV16/NETV16_CANDriver.c  
	@-${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F5015_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d -o ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ../NETVProtocolStack/NETV16/NETV16_CANDriver.c    2>&1  > ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err ; if [ $$? -eq 0 ] ; then touch ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok ; fi 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d > ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${CP} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp 
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d > ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${CP} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp
endif
	@touch ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err 
	@cat ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err 
	@if [ -f ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok ] ; then rm -f ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok; else exit 1; fi
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.ok ${OBJECTDIR}/main.o.err 
	@echo ${MP_CC} $(MP_EXTRA_CC_PRE)  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F5015_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF ${OBJECTDIR}/main.o.d -o ${OBJECTDIR}/main.o main.c  
	@-${MP_CC} $(MP_EXTRA_CC_PRE)  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F5015_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF ${OBJECTDIR}/main.o.d -o ${OBJECTDIR}/main.o main.c    2>&1  > ${OBJECTDIR}/main.o.err ; if [ $$? -eq 0 ] ; then touch ${OBJECTDIR}/main.o.ok ; fi 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/main.o.d > ${OBJECTDIR}/main.o.tmp
	@${RM} ${OBJECTDIR}/main.o.d 
	@${CP} ${OBJECTDIR}/main.o.tmp ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.tmp 
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/main.o.d > ${OBJECTDIR}/main.o.tmp
	@${RM} ${OBJECTDIR}/main.o.d 
	@${CP} ${OBJECTDIR}/main.o.tmp ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.tmp
endif
	@touch ${OBJECTDIR}/main.o.err 
	@cat ${OBJECTDIR}/main.o.err 
	@if [ -f ${OBJECTDIR}/main.o.ok ] ; then rm -f ${OBJECTDIR}/main.o.ok; else exit 1; fi
	
${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o: ../NETVProtocolStack/NETV16/NETV16_CANDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1908611077 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err 
	@echo ${MP_CC} $(MP_EXTRA_CC_PRE)  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F5015_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d -o ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ../NETVProtocolStack/NETV16/NETV16_CANDriver.c  
	@-${MP_CC} $(MP_EXTRA_CC_PRE)  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -D_DSPIC30F5015_ -I"." -I"../NETVProtocolStack/NETV16" -Os -MMD -MF ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d -o ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o ../NETVProtocolStack/NETV16/NETV16_CANDriver.c    2>&1  > ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err ; if [ $$? -eq 0 ] ; then touch ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok ; fi 
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d > ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${CP} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp 
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d > ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${CP} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.tmp
endif
	@touch ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err 
	@cat ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.err 
	@if [ -f ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok ] ; then rm -f ${OBJECTDIR}/_ext/1908611077/NETV16_CANDriver.o.ok; else exit 1; fi
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.elf: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf  -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.elf ${OBJECTFILES}        -Wl,--defsym=__MPLAB_BUILD=1,--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.elf: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf  -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.elf ${OBJECTFILES}        -Wl,--defsym=__MPLAB_BUILD=1,--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}/pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/dsPICBootloader.X.${IMAGE_TYPE}.elf -omf=elf
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/dsPIC30F5015
	${RM} -r dist/dsPIC30F5015

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
