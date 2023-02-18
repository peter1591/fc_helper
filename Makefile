FLAGS=-fno-exceptions

all:
	g++ ${FLAGS} -O3 src/main.cpp

debug:
	g++ ${FLAGS} -g src/main.cpp
