#include "UI.hpp"



inline GLFWwindow* init_UI()     
{
    if(!glfwInit())
    {
        std::cerr<<"Failed to initialize GLFW."<<std::endl;
        return nullptr;
    }
    
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    

    GLFWwindow* window = glfwCreateWindow(1280, 720, "GLFW Example", NULL, NULL);
    if(window == NULL) return nullptr;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();   


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

inline void destroy_UI(GLFWwindow *window)
{     
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief Handles the loading of OPGL-TEXTURE and the GUI logic
 */
inline void process_and_display_tex(GLFWwindow* window, const char* file_path)
{
    IMG_PROP img;
    img.texture = 0;
    if(pngLoader.Load(file_path, img.image))
    {
        std::cout << "Image Loaded! Uploading to GPU..." << std::endl;
        img.image.editedPixels = img.image.pixels;
        img.texture = UploadTexture(img.image);
    }
    else
    {
        std::cerr << "Failed to load image!" << std::endl;
    }

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Image Viewer");

        if(img.texture)
        {
            ImGui::Text("Size: %d x %d", img.image.width, img.image.height);
            ImGui::Image((void*)(intptr_t)img.texture, ImVec2(img.image.width, img.image.height));
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();
            ImGui::Spacing();

            bool is_brightness = ImGui::SliderInt("Brightness", &img.brightness, -255, 255);
            bool is_contrast = ImGui::SliderFloat("Contrast", &img.contrast, 0.f, 2.f);
            bool is_saturation = ImGui::SliderFloat("Saturation", &img.saturation, 0.f, 2.f);
            
            if(is_brightness || is_contrast || is_saturation)
            {
                processImage(img.image, img.brightness, img.contrast, img.saturation);
                UpdateTexture(img.texture, img.image);
            }

            if(ImGui::Button("Reset"))
            {
                img.brightness = 0;
                img.contrast = 1.f;
                img.saturation = 1.f;
                processImage(img.image, img.brightness, img.contrast, img.saturation);
                UpdateTexture(img.texture, img.image);
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

    if(img.texture)
    {
        glDeleteTextures(1, &img.texture);
    }
}


