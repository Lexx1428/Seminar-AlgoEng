# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Source files
SRCS = main.cpp allfunctions.cpp types.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = main

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compiling C++ source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleaning up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (not actual files)
.PHONY: all clean
