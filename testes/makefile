INCLUDE_PATH:=../include
INCLUDE:=-I${INCLUDE_PATH}

all: main plugin.so

loader.o: ../src/loader/loader.cpp ${INCLUDE_PATH}/interface.h
	g++ ${INCLUDE} $< -c -o $@ -ldl

main: main.cpp ${INCLUDE_PATH}/interface.h loader.o
	g++ ${INCLUDE} $< loader.o -o $@ -ldl 

plugin.so: plugin.cpp ${INCLUDE_PATH}/interface.h
	g++ ${INCLUDE} --shared -fPIC $< -o $@

clear:
	-rm -f loader.o main plugin.so

.PHONY: clear
