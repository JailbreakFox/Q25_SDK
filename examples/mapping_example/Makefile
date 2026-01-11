CXX = g++
CXXFLAGS = -std=c++11 -O2 -I../../include
LDFLAGS = -L../../lib/linux/x64 -lrobot_sdk -pthread -ldl

TARGET = mapping_example
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
