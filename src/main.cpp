#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <iostream>
#include "parser/PNGloader.h"

PNGloader pngLoader;
Image image;
int brightness;
float saturation = 1.f;
float contrast = 1.f;

int clamp_value(int value)
{
    if (value > 255)
    {
        return 255;
    }
    else if (value < 0)
    {
        return 0;
    }
    else
    {
        return value;
    }
}

void processImage(Image& img, int brightness = 0, float contrast = 1.f, float saturation =1.f )
{
    img.editedPixels = img.pixels;
    int bytesPerPixel = img.channels;
    float l_saturation;
    float m_contrast = 128;
    for(int i = 0; i < img.editedPixels.size(); i+=bytesPerPixel)
    {
        int colorChannels = (bytesPerPixel == 4) ? 3 : bytesPerPixel;
        l_saturation = 0.299f*img.editedPixels[i] + 0.587f*img.editedPixels[i+1] + 0.114f*img.editedPixels[i+2];
        for(int c = 0; c < colorChannels; c++)
        {
            int value = img.editedPixels[i + c] + brightness;
            value = clamp_value(value);
            img.editedPixels[i + c] = value;
            value = (int)(img.editedPixels[i+c] - m_contrast) * contrast + m_contrast;
            value = clamp_value(value);
            img.editedPixels[i+c] = value;
            value = (int)(img.editedPixels[i+c] - l_saturation)*saturation + l_saturation;
            value = clamp_value(value);
            img.editedPixels[i+c] = value;
        }
    }
}

/**
    @brief Uploads the texture to the window
    @param img The image to be uploaded
*/
GLuint UploadTexture(const Image& img)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Removes any padding - data as it is (OPGL ASSUMES 4 BYTES PER ROW)
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Tells OPGL the exact image channels
    GLenum format = (img.channels == 4) ? GL_RGBA : GL_RGB; 

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 img.width, img.height,
                 0, format,
                 GL_UNSIGNED_BYTE,
                 img.editedPixels.data());
    
    return textureID;
}

void UpdateTexture(GLuint textureID,const Image& img) 
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLenum format = (img.channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 img.width, img.height,
                 0, format,
                 GL_UNSIGNED_BYTE,
                 img.editedPixels.data());
}

int main()
{

    if(!glfwInit())
    {
        std::cerr<<"Failed to initialize GLFW."<<std::endl;
        return -1;
    }
    
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    

    GLFWwindow* window = glfwCreateWindow(1280, 720, "GLFW Example", NULL, NULL);
    if(window == NULL) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    GLuint myTexture = 0;

    if(pngLoader.Load("dogs.png", image))
    {
        std::cout << "Image Loaded! Uploading to GPU..." << std::endl;
        image.editedPixels = image.pixels;
        myTexture = UploadTexture(image);
        
    }
    else
    {
        std::cerr << "Failed to load image!" << std::endl;
        return -1;
    }
    

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Image Viewer");

        if(myTexture)
        {
            ImGui::Text("Size: %d x %d", image.width, image.height);
            ImGui::Image((void*)(intptr_t)myTexture, ImVec2(image.width, image.height));
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();
            ImGui::Spacing();
            
            if(ImGui::SliderInt("Brightness", &brightness, -255, 255) || ImGui::SliderFloat("Contrast", &contrast, 0.f, 2.f)||ImGui::SliderFloat("Saturation", &saturation, 0.f, 2.f))
            {
                processImage(image, brightness, contrast, saturation);
                UpdateTexture(myTexture, image);
            }

            if(ImGui::Button("Reset"))
            {
                brightness = 0;
                contrast = 1.f;
                saturation = 1.f;
                processImage(image, brightness, contrast, saturation);
                UpdateTexture(myTexture, image);
            }
        }
        else
        {
            ImGui::Text("Failed to load image!");
        }

        ImGui::End();
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }

    if(myTexture)
    {
        glDeleteTextures(1, &myTexture);
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}