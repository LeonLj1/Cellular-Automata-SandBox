CXX = g++
CXXFLAGS = -Wall -g -Iinclude
LIBS = -lSDL2 -lSDL2_ttf

TARGET = main

SRC = src/main.cpp src/particles.cpp
OBJ = build/main.o build/particles.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LIBS)

build/%.o: src/%.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

.PHONY: all clean