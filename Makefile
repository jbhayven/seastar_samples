CXX = c++
CXXFLAGS = -g -Wall -Wextra `pkg-config --cflags --static seastar`
LDLIBS =`pkg-config --libs --static seastar`

SOURCES=$(wildcard *.cpp)
EXECUTABLES=$(patsubst %.cpp,%,$(SOURCES))

all: $(EXECUTABLES)

clean:
	rm -f *.o *~ $(EXECUTABLES) 
