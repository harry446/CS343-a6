CXX = u++                     # uC++ compiler
CXXFLAGS = -g -Wall -Wextra -multi -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

# List *all* .cc files in your folder
SOURCES = config.cc bank.cc bottlingPlant.cc groupoff.cc \
          nameServer.cc parent.cc printer.cc q6main.cc \
          soda.cc student.cc truck.cc vendingMachine.cc \
          watCard.cc watCardOffice.cc

OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${SOURCES:.cc=.d}

EXEC = MySoda

ifeq ($(notdir $(strip ${CXX})),cfa)
CXXFLAGS := $(filter-out -multi,${CXXFLAGS})
CXXFLAGS += "-Wno-implicit-fallthrough"
.SUFFIXES:
.SUFFIXES: .cfa .o
.cfa.o:
	${CXX} ${CXXFLAGS} -c $<
endif

all: ${EXEC}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS}: ${MAKEFILE_NAME}

-include ${DEPENDS}

clean:
	rm -f ${EXEC} *.o *.d
