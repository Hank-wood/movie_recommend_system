.PHONY: clean
.PHONY:	main

CXXFLAGS = `pkg-config --cflags --libs gtkmm-3.0` -std=c++11
SRC = $(wildcard ./*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

main: $(OBJ)
	g++ $(OBJ) $(CXXFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	-rm *.o
