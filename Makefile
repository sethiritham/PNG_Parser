CXX      := g++
CC       := gcc               # Added: The C compiler
CXXFLAGS := -Wall -Wextra -std=c++17 -O2
CFLAGS   := -O2               # Added: Flags for C files (keep it simple for vendor libs)

# Add -MMD -MP to generate dependency files automatically
CPPFLAGS := -MMD -MP

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/editor
SRC_DIR   := src

# 1. Find C++ files
CPP_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
# 2. Find C files (like miniz.c)
C_SRCS   := $(shell find $(SRC_DIR) -name '*.c')

# 3. Map src/%.cpp -> build/%.o
CPP_OBJS := $(CPP_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# 4. Map src/%.c   -> build/%.o
C_OBJS   := $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Combine all objects
OBJS := $(CPP_OBJS) $(C_OBJS)

# Dependency files derived from all object files
DEPS := $(OBJS:.o=.d)

LIBS := -lz -lglfw -lGL -ldl

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Rule for C++ source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

# Rule for C source (Added this block)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Include the dependency files
-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)