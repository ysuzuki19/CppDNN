PROG = main.cpp
TARGET = proj
HEADER = ../header
MAKEFILE = makefile
CXX = g++
CXX_VERSION = -std=c++17
CXX_OPTION = -Wall -o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
#LIB_FS = -lstdc++fs # NEED: if you use UBUNTU <= 18.04
else
LIB_FS =
endif

${TARGET} : ${PROG} ${HEADER} ${MAKEFILE}
	${CXX} ${CXX_VERSION} ${CXX_OPTION} ${TARGET} -I${HEADER} ${PROG} ${LIB_FS}

run :
	./proj

clean :
	rm -r ${TARGET}
