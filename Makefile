# Variables
COMPILER = g++
CFLAGS = -Wall -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
INCLUDES = -I"./HEADER"

SRC_DIR = ./SRC
HEADER_DIR = ./HEADER
OBJ_DIR = ./BIN
TARGET = ./PROGRAM/demo.exe

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)

# Convert .cpp files to .o files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP_FILES = $(OBJ_FILES:.o=.d)

.PHONY: all run build clean

all: build

run: build
	@$(TARGET)

runServer: build
	@$(TARGET) "server"

runClient: build
	@$(TARGET) "client"

build: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	@echo "Linking objects to executable..."
	@mkdir -p $(dir $@)
	$(COMPILER) -g $^ -o $@ $(LDFLAGS) $(INCLUDES)
	@echo "Build completed successfully."

# Compile each .cpp file into .o and generate dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	$(COMPILER) -g $(CFLAGS) -c $< -o $@ $(INCLUDES) 
	$(COMPILER) -MM $(CFLAGS) $(INCLUDES) $< | sed 's|$(notdir $<)|$@|' > $(OBJ_DIR)/$*.d

# Include dependency files
-include $(DEP_FILES)

# Clean build files
clean:
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned build files."
