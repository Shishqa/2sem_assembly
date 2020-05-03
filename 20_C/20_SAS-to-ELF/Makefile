CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN	:= bin
SRC	:= src

#INCLUDE	:= ../old_libs
#LIB		:= ../old_libs/mslib

#LIBRARIES	:= -lMSLib
EXECUTABLE	:= sastoelf64

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@ 

clean:
	-rm *.o
