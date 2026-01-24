#include <GLFW/glfw3.h>
#include <iostream>

void dropCallback(GLFWwindow* window, int count, const char** paths)
{
    for(int i = 0; i < count; i++)
    {
        std::cout<<"Dropped file: "<<paths[i]<<std::endl;
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

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}