#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Auxiliar/MapResizer.o \
	${OBJECTDIR}/DataBase/DaoGeneral.o \
	${OBJECTDIR}/DataBase/DaoTask.o \
	${OBJECTDIR}/PathPlanning/DStartLite.o \
	${OBJECTDIR}/PathPlanning/PriorityQueue.o \
	${OBJECTDIR}/TasksPlanning/TaskPlanner.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-D STATISTICS
CXXFLAGS=-D STATISTICS

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread -L/usr/local/lib64 -lplayerc++ -lboost_thread -lboost_signals -lm -lplayerc -lm -lz -lplayerinterface -lplayerwkb -lplayercommon -lboost_system -L/usr/lib -lmysqlcppconn

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hospitallogisticsrobot

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hospitallogisticsrobot: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hospitallogisticsrobot ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Auxiliar/MapResizer.o: Auxiliar/MapResizer.cpp
	${MKDIR} -p ${OBJECTDIR}/Auxiliar
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/local/include/player-3.0 -I/usr/include/cppconn -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Auxiliar/MapResizer.o Auxiliar/MapResizer.cpp

${OBJECTDIR}/DataBase/DaoGeneral.o: DataBase/DaoGeneral.cpp
	${MKDIR} -p ${OBJECTDIR}/DataBase
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/local/include/player-3.0 -I/usr/include/cppconn -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DataBase/DaoGeneral.o DataBase/DaoGeneral.cpp

${OBJECTDIR}/DataBase/DaoTask.o: DataBase/DaoTask.cpp
	${MKDIR} -p ${OBJECTDIR}/DataBase
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/local/include/player-3.0 -I/usr/include/cppconn -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DataBase/DaoTask.o DataBase/DaoTask.cpp

${OBJECTDIR}/PathPlanning/DStartLite.o: PathPlanning/DStartLite.cpp
	${MKDIR} -p ${OBJECTDIR}/PathPlanning
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/local/include/player-3.0 -I/usr/include/cppconn -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PathPlanning/DStartLite.o PathPlanning/DStartLite.cpp

${OBJECTDIR}/PathPlanning/PriorityQueue.o: PathPlanning/PriorityQueue.cpp
	${MKDIR} -p ${OBJECTDIR}/PathPlanning
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/local/include/player-3.0 -I/usr/include/cppconn -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PathPlanning/PriorityQueue.o PathPlanning/PriorityQueue.cpp

${OBJECTDIR}/TasksPlanning/TaskPlanner.o: TasksPlanning/TaskPlanner.cpp
	${MKDIR} -p ${OBJECTDIR}/TasksPlanning
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/local/include/player-3.0 -I/usr/include/cppconn -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TasksPlanning/TaskPlanner.o TasksPlanning/TaskPlanner.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/local/include/player-3.0 -I/usr/include/cppconn -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
