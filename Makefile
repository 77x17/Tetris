# Variables
COMPILER = g++
CFLAGS = -Wall -g -MMD -MP
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system 
INCLUDES = -I"./HEADER" -I"./HEADER/BLOCK" -I"./HEADER/COMPONENT" -I"./HEADER/PLAYER" -I"./HEADER/MODEPLAY" -I"./HEADER/MONITOR" -I"./HEADER/MOVEMENTCONTROLLER" -I"./HEADER/CONTROLLER"

SRC_DIR = ./SRC
OBJ_DIR = ./BIN
TARGET = ./PROGRAM/demo.exe

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*/*.cpp)

# Convert .cpp files to .o files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Dependency files
DEP_FILES = $(OBJ_FILES:.o=.d)

.PHONY: all run build clean

all: build

run: build
	@$(TARGET)

build: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	@echo "Linking objects to executable..."
	@mkdir -p $(dir $@)
	@$(COMPILER) -g $^ -o $@ $(LDFLAGS) $(INCLUDES)
	@echo "Build completed successfully."

# Compile each .cpp file into .o and generate dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(COMPILER) $(CFLAGS) -c $< -o $@ $(INCLUDES)


# Include dependency files
-include $(DEP_FILES)

# # Clean build files
clean:
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned build files."
