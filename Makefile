CXX := g++
CXXFLAGS := -O0 -g -Wall -std=c++11 -Werror=return-type

main.out: Transformer.cpp Evaluator.cpp main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test: main.out
	valgrind --leak-check=full ./main.out

clean:
	rm -f *.out
