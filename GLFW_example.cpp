#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


void dropCallback(GLFWwindow* window, int count, const char** paths)
{
    for(int i = 0; i < count; i++)
    {
        std::cout<<"Dropped file: "<<paths[i]<<std::endl;
    }
}

void frameBufferCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void closeCallback(GLFWwindow* window)
{
    std::cout<<"Window closed."<<std::endl;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    if(!glfwInit())
    {
        std::cerr<<"Failed to initialize GLFW."<<std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "GLFW Example", NULL, NULL);
    if(!window)
    {   
        std::cerr<<"Failed to create GLFW window."<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetDropCallback(window, dropCallback);
    glfwSetWindowCloseCallback(window, closeCallback);

    glfwMakeContextCurrent(window);



    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr<<"Failed to initialize GLAD."<<std::endl;
        return -1;
    }
    

    glViewport(0, 0, 640, 480);
    glfwSetFramebufferSizeCallback(window, frameBufferCallback);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}