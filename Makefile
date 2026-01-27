CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

TARGET = editor
SRC_DIR = src
BUILD_DIR = build

SRCS = \
	src/main.cpp \
	src/parser/PNGloader.cpp \
	src/vendor/imgui/imgui.cpp \
	src/vendor/imgui/imgui_draw.cpp \
	src/vendor/imgui/imgui_tables.cpp \
	src/vendor/imgui/imgui_widgets.cpp \
	src/vendor/imgui/imgui_impl_glfw.cpp \
	src/vendor/imgui/imgui_impl_opengl3.cpp

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

LIBS = -lz -lglfw -lGL -ldl

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
