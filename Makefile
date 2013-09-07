CC = g++
CCFLAGS = -Wall -O2

all: lib

lib: objects
	if [ ! -d lib ]; then mkdir lib; fi
	ar rvs lib/terminator.a *.o

objects:
	$(CC) $(CCFLAGS) -c src/terminator*.cc

clean:
	rm -rf *.o
	if [ -d lib ]; then rm -rf lib/*; fi

format:
	cd src; astyle --style=ansi --indent=spaces=2 *.cc *.h; rm *.orig
