CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2
# Add -MMD -MP to generate dependency files automatically
CPPFLAGS := -MMD -MP

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/editor
SRC_DIR   := src

# reliably find all .cpp files inside src/ recursively
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

# Map src/path/to/file.cpp -> build/path/to/file.o
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Dependency files derived from object files
DEPS := $(OBJS:.o=.d)

LIBS := -lz -lglfw -lGL -ldl

# Include paths (optional, but usually needed so you can do #include "header.h" 
# regardless of where the file is located)
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Compile C++ source
# We add $(CPPFLAGS) for dependency generation and $(INC_FLAGS) for header includes
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

# Include the dependency files (if they exist) so Make knows about header changes
-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)