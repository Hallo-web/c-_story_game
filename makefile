# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
# Output executable name
TARGET = main
# Source files
SRCS = main.cpp
# Object files (same names as SRCS but with .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule: build everything
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Optional: shortcut to run the program
run: all
	./$(TARGET)
