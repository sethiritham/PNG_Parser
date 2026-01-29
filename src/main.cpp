#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <iostream>
#include "parser/PNGloader.h"
#include "graphics/UI.hpp"
#include "graphics/editor.hpp"
#include "graphics/texture.hpp"

int main()
{
    GLFWwindow *window;
    window = init_UI();
    const char* file_path = "assets/sculpture.png";
    process_and_display_tex(window, file_path);
    destroy_UI(window);
    
    return 0;
}