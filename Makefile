#Variables
COMPILER = g++
CFLAGS = -Wall -g
LDFLAGS = -lncurses -I"./HEADER"

SRC_DIR = ./SRC
HEADER_DIR = ./HEADER
OBJ_DIR = ./BIN
TARGET = ./PROGRAM/demo.exe

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)

# Convert .cpp files to .o files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

run: $(TARGET)
	@$(TARGET)

build: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	@echo "Linking objects to executable..."
	@mkdir -p $(dir $@)
	@$(COMPILER) $(OBJ_FILES) -o $@ $(LDFLAGS)
	@echo "Build completed successfully."

# Compile each .cpp file into .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(COMPILER) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Clean build files
clean:
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned build files."


# Dependency rules (optional but improves clarity)
$(OBJ_DIR)/COMPONENTS/Map.o: $(HEADER)/Common.hpp $(HEADER)/Map.hpp

.PHONY: all clean run build
