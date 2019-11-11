PROG = main.cpp
TARGET = proj
HEADER = ./main.cpp
CXX = g++
CXX_VERSION = -std=c++17

${TARGET} : ${PROG} ${HEADER}
	${CXX} ${CXX_VERSION} -o ${TARGET} ${PROG}

run :
	./proj

clean :
	rm -r ${TARGET}
