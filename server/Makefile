CXX = g++
#CXXFLAGS = -std=c++1y -fsanitize=address -O2
CXXFLAGS = -std=c++1y -O2

SRC = $(wildcard ./*.cpp) ../common/package.cpp ml/pred.cpp
OBJ = $(patsubst %.cpp, %.o, $(SRC))

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -lmysqlcppconn -lboost_system

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@ -c

clean:
	@$(RM) $(OBJ)
