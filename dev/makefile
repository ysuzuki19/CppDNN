PROG = main.cpp
TARGET = proj
HEADER = ../header
CXX = g++
CXX_VERSION = -std=c++17
CXX_OPTION = -o

ifeq ($(OS), Linux)
CXX_LIBRARY = -lstdc++fs
else
CXX_LIBRARY =
endif

${TARGET} : ${PROG} ${HEADER}
	${CXX} ${CXX_VERSION} ${CXX_OPTION} ${TARGET} -I${HEADER} ${PROG} ${CXX_LIBRARY}

run :
	./proj

clean :
	rm -r ${TARGET}
