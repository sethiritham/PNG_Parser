#ifndef UI_HPP
#define UI_HPP

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <iostream>
#include "editor.hpp"
#include "texture.hpp"

inline GLFWwindow* init_UI();
inline void destroy_UI(GLFWwindow *window);
inline void process_and_display_tex(GLFWwindow* window, const char* file_path);

#endif