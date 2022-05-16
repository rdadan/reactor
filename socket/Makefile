INC_DIR:= ./include/socket # ./inc/net/ ./inc/threadpool/ ./inc/timer/ ./inc/cache/
SRC_DIR:= ./source/socket # ./src/net ./src/threadpool ./src/timer ./src/cache/
SRCS:=$(wildcard ./source/socket/*.cpp) #$(wildcard ./src/net/*.cc) $(wildcard ./src/net/*.c) $(wildcard ./src/threadpool/*.cc) $(wildcard ./src/timer/*.cc) $(wildcard ./src/cache/*.cc)
OBJS:= $(patsubst %.cpp, %.o, $(SRCS))
LIBS:= -lpthread

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./ser

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
