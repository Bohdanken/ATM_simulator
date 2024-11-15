#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "imgui_util.h"
#include "glfw_config.h"
#include "window_config.h"

#include <string>

namespace ATM::Util
{
    void imgui_init(GLFWwindow *const window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        // Specify your OpenGL version
        std::string opengl_version("#version ");
        opengl_version += (Config::GL_VERSION_MAJOR + '0');
        opengl_version += (Config::GL_VERSION_MINOR + '0');
        opengl_version += '0';
        ImGui_ImplOpenGL3_Init(opengl_version.c_str());
    }

    void imgui_start_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void imgui_render_frame(GLFWwindow *const window)
    {
        // Define previous window width and height
        static int32_t prev_win_w = -1, prev_win_h = -1;

        // Prepare the frame for rendering
        ImGui::Render();
        int32_t win_w, win_h;
        glfwGetFramebufferSize(window, &win_w, &win_h);

        // Update OpenGL viewport if a window resize has occurred
        if (win_w != prev_win_w || win_h != prev_win_h)
        {
            glViewport(0, 0, win_w, win_h);
            prev_win_w = win_w;
            prev_win_h = win_h;
        }

        // Clear the background
        glClearColor(
            Config::WIN_CLEAR.RED,
            Config::WIN_CLEAR.GREEN,
            Config::WIN_CLEAR.BLUE,
            Config::WIN_CLEAR.ALPHA
        );
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the frame
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void imgui_cleanup()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
