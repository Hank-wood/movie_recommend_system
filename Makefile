CXX = clang++
CXXFLAGS = -g -std=c++11 -fsanitize=undefined

SRC = $(wildcard ./*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

PROG = rd

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(PROG) -lmysqlcppconn

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@ -c

clean:
	@$(RM) *.o
