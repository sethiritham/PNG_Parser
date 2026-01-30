#ifndef TEXTURE_H
#define TEXTURE_H
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include "parser/PNGloader.h"

typedef struct
{
    int brightness = 0;
    float saturation = 1.f;
    float contrast = 1.f;
    Image image;
    GLuint texture;
}IMG_PROP;

extern PNGloader pngLoader;
GLuint UploadTexture(const Image& img);
void UpdateTexture(GLuint textureID,const Image& img);


#endif 
