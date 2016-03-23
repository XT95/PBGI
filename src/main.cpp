#define GLEW_STATIC
#ifdef __WIN32__
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "demo.h"

unsigned screenX = 1280;
unsigned screenY = 720;

GLFWwindow *window = NULL;
GLFWmonitor *monitor = NULL;
Demo *demo = NULL;

void window_size_callback(GLFWwindow* window, int width, int height)
{
    (void) window;
    screenX = width;
    screenY = height;
    demo->setResolution(width, height);
}

void window_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(screenX/2 != xpos && screenY != ypos)
    {
        demo->setMouseMovement(screenX/2-xpos, screenY/2-ypos);
        glfwSetCursorPos(window, screenX/2, screenY/2);
    }
}

void window_mouse_callback(GLFWwindow* window, int button, int action, int mod)
{
    (void) mod;

    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetCursorPos(window, screenX/2, screenY/2);
            glfwSetCursorPosCallback(window, window_cursor_position_callback);
        }
        else
        {
            glfwSetCursorPosCallback(window, NULL);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void window_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) mods;
    if(action != GLFW_RELEASE)
    {
        switch(key)
        {
        case (GLFW_KEY_ESCAPE):
            glfwSetWindowShouldClose(window, 1);
            break;
        case (GLFW_KEY_R):
            demo->placeLightToCamera();
            break;
        case GLFW_KEY_D:
            demo->moveCameraRight();
            break;
        case GLFW_KEY_A:
            demo->moveCameraLeft();
            break;
        case GLFW_KEY_S:
            demo->moveCameraBackward();
            break;
        case GLFW_KEY_W:
            demo->moveCameraForward();
            break;
        case GLFW_KEY_SPACE:
            demo->reloadShader();
            break;
        }
    }
}

void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void) window;
    (void) xoffset;
    demo->modifyFov(yoffset);
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    // GLFW Window init
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    window = glfwCreateWindow(screenX, screenY, "PBGI", NULL, NULL);

    if(!window)
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 4.1?");
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, window_mouse_callback);
    glfwSetKeyCallback(window, window_keyboard_callback);
    glfwSetScrollCallback(window, window_scroll_callback);

    // GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    //Get some infos about the GPU
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    if(!GLEW_VERSION_4_3)
        throw std::runtime_error("OpenGL 4.3 API is not available.");

    //Demo
    demo = new Demo(screenX, screenY);

    //Mainloop
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        demo->render();
        //demo->showDebug();
        glfwSwapBuffers(window);
    }

    delete demo;
    glfwTerminate();
    return EXIT_SUCCESS;
}
