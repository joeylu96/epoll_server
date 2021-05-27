INC_DIR:= /usr/include/jsoncpp tool frame
SRCS:=$(wildcard ./*.cpp tool/*.cpp frame/*.cpp)
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))

LIBS:=-lpthread -ljsoncpp

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wl,-rpath="/usr/local/lib"

EXE:= ./simple_server

$(EXE):$(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
