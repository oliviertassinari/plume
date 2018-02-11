LDLIBS= -lm -lcblas -lgsl -lstdc++ -lpthread
CFLAGS= -g -O2 -std=gnu99 -Wall -Wextra -Werror
CXXFLAGS= -g -O2 -std=c++0x -Wall -Wextra -Werror
OBJS=test_main.o dof.o magnetic.o matrix_utils.o TrajectorySimulation.o PlumeReceiver.o PlumeEmitter.o CircularBuffer.o polynomial_regression.o debug_utils.o

all: test

debug: CFLAGS+= -DDEBUG
debug: CXXFLAGS+= -DDEBUG
debug: test

test: $(OBJS)
	g++ $^ $(LDLIBS) -o $@

clean: 
	rm -f *.o
	rm -f test

