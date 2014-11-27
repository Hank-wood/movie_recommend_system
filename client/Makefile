.PHONY: clean
.PHONY:	main

#CXXFLAGS = `pkg-config --cflags --libs gtkmm-3.0` -std=c++1y -g
CXXFLAGS = `pkg-config --cflags --libs gtkmm-3.0` -std=c++1y -O2
SRC = $(wildcard ./*.cpp) ../common/package.cpp
OBJ = $(patsubst %.cpp, %.o, $(SRC))

main: $(OBJ)
	g++ $(OBJ) $(CXXFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)
