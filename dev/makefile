PROG = main.cpp
TARGET = proj
HEADER = ../header
CXX = g++
CXX_VERSION = -std=c++17

${TARGET} : ${PROG} ${HEADER}
	${CXX} ${CXX_VERSION} -o ${TARGET} -I ${HEADER} ${PROG}

run :
	./proj

clean :
	rm -r ${TARGET}
