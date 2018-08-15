include ../Make.inc

CXXFLAGS-add+= -I../include

SRCS =  \
confidence_interval.cpp \
dating.cpp \
estimate_root.cpp \
options.cpp \
readData.cpp \
utils.cpp

OBJS = $(SRCS:%.cpp=%.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS-add) -c $< -o $@

release debug: ../lib/liblsd.$(SHLIB_EXT)

../lib/liblsd.$(SHLIB_EXT): $(OBJS)
	rm -rf $@
	$(CXX) $(LDFLAGS) $(LDFLAGS-add) -shared -o $@ $^

clean:
	rm -f *.o ../lib/*.$(SHLIB_EXT) 
