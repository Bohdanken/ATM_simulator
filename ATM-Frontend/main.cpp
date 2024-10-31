#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "glfw_util.h"
#include "imgui_util.h"

#include <iostream>

static void renderUI()
{
    ImGui::Text("Hello, ImGui!");
    ImGui::SliderFloat("Float", new float(0.0f), 0.0f, 1.0f);
}

using namespace ATM;

int main()
{
    // Init
    Util::glfw_init(4, 6);
    GLFWwindow *const window = Util::window_init(1600, 900, "ATM Frontend");
    Util::glfw_log_info();
    Util::imgui_init(window);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render your UI
        renderUI();

        // Rendering
        ImGui::Render();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    Util::imgui_cleanup();
    Util::glfw_cleanup(window);
    return 0;
}
