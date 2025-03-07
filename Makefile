# Variables
COMPILER = g++
CFLAGS = -Wall -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
INCLUDES = -I"./HEADER"

SRC_DIR = ./SRC
HEADER_DIR = ./HEADER
OBJ_DIR = ./BIN
TARGET = ./PROGRAM/demo.exe

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)

# Convert .cpp files to .o files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

.PHONY: all run build clean

all: build

run: build
	@$(TARGET)

build: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	@echo "Linking objects to executable..."
	@mkdir -p $(dir $@)
	$(COMPILER) -g $^ -o $@ $(LDFLAGS) $(INCLUDES)
	@echo "Build completed successfully."

# Compile each .cpp file into .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(COMPILER) -g $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Clean build files
clean:
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned build files."

# Dependency rules (optional but improves clarity)

$(OBJ_DIR)/main.o: $(HEADER_DIR)/Common.hpp $(HEADER_DIR)/Tetris.hpp $(HEADER_DIR)/Block.hpp
$(OBJ_DIR)/MONITOR/Tetris.o: $(HEADER_DIR)/Tetris.hpp $(HEADER_DIR)/Player.hpp
$(OBJ_DIR)/MONITOR/Player.o: $(HEADER_DIR)/Player.hpp $(HEADER_DIR)/LinkListBlock.hpp $(HEADER_DIR)/Map.hpp $(HEADER_DIR)/CurrentBlock.hpp
$(OBJ_DIR)/MONITOR/Monitor.o: $(HEADER_DIR)/Monitor.hpp $(HEADER_DIR)/Hold.hpp $(HEADER_DIR)/Map.hpp $(HEADER_DIR)/Infor.hpp $(HEADER_DIR)/CurrentBlock.hpp $(HEADER_DIR)/LinkListBlock.hpp
$(OBJ_DIR)/COMPONENTS/Map.o: $(HEADER_DIR)/Common.hpp $(HEADER_DIR)/CurrentBlock.hpp $(HEADER_DIR)/Block.hpp
$(OBJ_DIR)/COMPONENTS/LinkListBlock.o: $(HEADER_DIR)/LinkListBlock.hpp $(HEADER_DIR)/Block.hpp $(HEADER_DIR)/BlockFactory.hpp $(HEADER_DIR)/CurrentBlock.hpp
$(OBJ_DIR)/COMPONENTS/Infor.o: $(HEADER_DIR)/Infor.hpp
$(OBJ_DIR)/COMPONENTS/Hold.o: $(HEADER_DIR)/Hold.hpp $(HEADER_DIR)/Block.hpp
$(OBJ_DIR)/COMPONENTS/CurrentBlock.o: $(HEADER_DIR)/CurrentBlock.hpp $(HEADER_DIR)/Common.hpp $(HEADER_DIR)/Block.hpp $(HEADER_DIR)/Map.hpp  $(HEADER_DIR)/Hold.hpp 
$(OBJ_DIR)/COMPONENTS/BlockFactory.o: $(HEADER_DIR)/BlockFactory.hpp $(HEADER_DIR)/Block.hpp
$(OBJ_DIR)/COMPONENTS/Block.o: $(HEADER_DIR)/Block.hpp $(HEADER_DIR)/Common.hpp