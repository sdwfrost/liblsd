include ../Make.inc

CXXFLAGS-add += -I../include
LDFLAGS-add += -L../lib -llsd

all: lsd

lsd.o: lsd.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS-add) $(fPIC) -c $< -o $@

lsd: lsd.o
	$(CXX) $(LDFLAGS)  -o $@ $^ $(LDFLAGS-add)

clean:
	rm -f lsd.o lsd
