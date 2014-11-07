SRCS = $(wildcard *.cpp)

SRC_O = $(patsubst %.cpp,%.o,$(SRCS))

temp: $(SRC_O)
	g++ $(SRC_O) -o a.out -std=c++11 -g -pthread -lgtest -lgtest_main 

%.o: %.cpp
	g++ -c $< -std=c++11

clean:
	rm a.out
