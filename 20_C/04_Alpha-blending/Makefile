CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -msse4.2

BIN	:= bin
SRC := src

EXECUTABLE := blender

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@

clean:
	-rm *.o
