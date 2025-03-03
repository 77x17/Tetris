# Variables
COMPILER = g++
CFLAGS = -Wall -g
LDFLAGS = -lncurses
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
# $(OBJ_DIR)/COMPONENTS/CurrentPiece.o: $(HEADER_DIR)/Common.hpp $(HEADER_DIR)/Piece.hpp $(HEADER_DIR)/CurrentPiece.hpp $(HEADER_DIR)/Screen.hpp
# $(OBJ_DIR)/COMPONENTS/Map.o: $(HEADER_DIR)/Common.hpp $(HEADER_DIR)/Map.hpp
# $(OBJ_DIR)/MONITOR/Monitor.o: $(HEADER_DIR)/Monitor.hpp $(HEADER_DIR)/Common.hpp $(HEADER_DIR)/Piece.hpp $(HEADER_DIR)/SetPiece.hpp $(HEADER_DIR)/Screen.hpp $(HEADER_DIR)/CurrentPiece.hpp
# $(OBJ_DIR)/COMPONENTS/Piece.o: $(HEADER_DIR)/Piece.hpp $(HEADER_DIR)/Common.hpp  $(HEADER_DIR)/Map.hpp $(HEADER_DIR)/Screen.hpp 
# $(OBJ_DIR)/COMPONENTS/Player.o: $(HEADER_DIR)/Player.hpp $(HEADER_DIR)/Monitor.hpp $(HEADER_DIR)/Screen.hpp
# $(OBJ_DIR)/SCREEN/Screen.o: $(HEADER_DIR)/Screen.hpp $(HEADER_DIR)/Common.hpp $(HEADER_DIR)/Piece.hpp $(HEADER_DIR)/SetPiece.hpp $(HEADER_DIR)/Map.hpp $(HEADER_DIR)/CurrentPiece.hpp
# $(OBJ_DIR)/COMPONENTS/SetPiece.o: $(HEADER_DIR)/SetPiece.hpp $(HEADER_DIR)/Piece.hpp $(HEADER_DIR)/Common.hpp $(HEADER_DIR)/Screen.hpp 
# $(OBJ_DIR)/MONITOR/Tetris.o: $(HEADER_DIR)/Tetris.hpp $(HEADER_DIR)/Player.hpp  


