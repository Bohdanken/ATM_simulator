#pragma once
#ifndef GLFW_UTIL_H
#define GLFW_UTIL_H

#include <GLFW/glfw3.h>

#include <cstdint>

namespace ATM::Util
{
    void glfw_init(const int32_t major, const int32_t minor, const bool run_compat = false);
    void glfw_cleanup(GLFWwindow *const window);
    GLFWwindow *window_init(const int32_t width, const int32_t height, const char *title);
    
    // Could be moved into a separate logging system, if time permits
    void glfw_log_info();
};

#endif // !GLFW_UTIL_H
