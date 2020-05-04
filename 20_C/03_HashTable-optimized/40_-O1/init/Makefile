CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -msse4.2 -O1

SRC := src

EXECUTABLE := HashTable

all: $(EXECUTABLE)

run: clean all
	clear
	./$(EXECUTABLE)

$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@

clean:
	-rm *.o
