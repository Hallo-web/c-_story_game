# Enhanced Makefile for C++ Story Game
# Compiler
CXX = g++

# Compiler flags - enhanced with optimization and debugging
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2 -g

# Libraries (if needed for threading or other features)
LIBS = -pthread

# Output executable name
TARGET = osiris_game

# Source files
SRCS = main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header dependencies (add as you create header files)
DEPS = 

# Default rule: build everything
all: $(TARGET)
	@echo "Build complete! Run with 'make run' or './$(TARGET)'"

# Link object files into the final executable
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Compile .cpp files to .o files
%.o: %.cpp $(DEPS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files and save games
clean:
	@echo "Cleaning build files..."
	rm -f $(OBJS) $(TARGET)
	@echo "Clean complete!"

# Clean everything including save files
clean-all: clean
	@echo "Removing save files..."
	rm -f savegame.txt enhanced_savegame.txt
	@echo "Full clean complete!"

# Run the program
run: $(TARGET)
	@echo "Starting OSIRIS Protocol..."
	@echo "=========================================="
	./$(TARGET)

# Debug build with extra debugging symbols
debug: CXXFLAGS += -DDEBUG -ggdb3
debug: $(TARGET)
	@echo "Debug build complete!"

# Release build with optimizations
release: CXXFLAGS += -DNDEBUG -O3 -march=native
release: clean $(TARGET)
	@echo "Release build complete!"

# Install to system (optional)
install: $(TARGET)
	@echo "Installing to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete! Run 'osiris_game' from anywhere."

# Uninstall from system
uninstall:
	@echo "Removing from /usr/local/bin..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstallation complete!"

# Check for memory leaks (requires valgrind)
memcheck: debug
	@echo "Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the game (default)"
	@echo "  run       - Build and run the game"
	@echo "  clean     - Remove build files"
	@echo "  clean-all - Remove build files and save games"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized release version"
	@echo "  install   - Install to system"
	@echo "  uninstall - Remove from system"
	@echo "  memcheck  - Check for memory leaks (requires valgrind)"
	@echo "  help      - Show this help message"

# Declare phony targets
.PHONY: all clean clean-all run debug release install uninstall memcheck help

# Automatic dependency generation (advanced)
-include $(OBJS:.o=.d)

%.d: %.cpp
	@$(CXX) $(CXXFLAGS) -MM $< > $@