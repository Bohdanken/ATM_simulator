#include "glfw_util.h"

#include <iostream>

namespace ATM::Util
{
    void glfw_init(const int32_t major, const int32_t minor, const bool run_compat)
    {
        // Initialise GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            std::exit(-1);
        }

        // Specify OpenGL version & operation mode
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        if (run_compat)
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        else 
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void glfw_cleanup(GLFWwindow *const window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow *window_init(const int32_t width, const int32_t height, const char *const title)
    {
        GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            std::exit(-1);
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        return window;
    }

    void glfw_log_info()
    {
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    }
};
