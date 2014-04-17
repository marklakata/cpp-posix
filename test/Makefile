INC = -I../include
CXXFLAGS=$(INC) -std=c++11
COMMON_SOURCES = ../src/impl-memory-map.cc ../src/impl-mqueue.cc ../src/impl-shared-memory.cc
COMMON_OBJECTS = $(patsubst %.cc,%.o,$(COMMON_SOURCES))
LDOPTS = -lrt

all:tst0 tst1 tst3


tst0: tst0.o $(COMMON_OBJECTS)
	$(CXX) -o $@ $^ $(LDOPTS)

tst1: tst1.o $(COMMON_OBJECTS)
	$(CXX) -o $@ $^ $(LDOPTS)

tst3: tst3.o $(COMMON_OBJECTS)
	$(CXX) -o $@ $^ $(LDOPTS)

clean:
	rm -f $(COMMON_OBJECTS) tst0.o tst1.o tst3.o