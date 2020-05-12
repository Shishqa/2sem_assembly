CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -fsanitize=address

EXECUTABLE	:= test_vector

all: $(EXECUTABLE)

run: clean all
	clear
	./$(EXECUTABLE)

$(EXECUTABLE): *.cpp 
	$(CXX) $(CXX_FLAGS) $^ -o $@ 

clean:
	-rm *.o
