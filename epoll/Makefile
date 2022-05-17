INC_DIR:= ./include/socket ./include/epoll 
SRC_DIR:= ./ ./source/socket ./include/epoll 
SRCS:=$(wildcard ./*.cpp) $(wildcard ./source/socket/*.cpp) $(wildcard ./source/epoll/*.cpp) 
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
