#pragma once
#ifndef IMGUI_UTIL_H
#define IMGUI_UTIL_H

#include <GLFW/glfw3.h>

#include <cstdint>

namespace ATM::Util
{
    void imgui_init(GLFWwindow *const window);
    void imgui_start_frame();
    void imgui_render_frame(GLFWwindow *const window);
    void imgui_cleanup();
}

#endif // !IMGUI_UTIL_H
