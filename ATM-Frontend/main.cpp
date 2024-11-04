#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "glfw_config.h"
#include "window_config.h"
#include "glfw_util.h"
#include "imgui_util.h"

#include <iostream>

using namespace ATM;

// Permissions
static bool is_authenticated = false;
static bool is_authorised = false;

// Frontend buffers
static char pin_input[5];      //  4 digits
static char account_input[13]; // 12 digits

static void renderUI()
{
    static ImGuiIO &io = ImGui::GetIO();

    const float half_width = io.DisplaySize.x / 2.0f;
    const float half_height = io.DisplaySize.y / 2.0f;

    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("ATM", nullptr, Config::MAIN_WIN_FLAGS);

    if (!is_authenticated) // is not authenticated && is not authorised
    {
        ImGui::Text("Enter your account number (12 digits):");
        ImGui::SetNextItemWidth(half_width * 0.35f);
        ImGui::InputText("##account number", account_input, IM_ARRAYSIZE(account_input));
        // ask for the card number
        // if card number fails, launch error pop-up
    }
    else if (!is_authorised) // is authenticated, but is not authorised
    {
        ImGui::Text("Enter your pin (4 digits):");
        ImGui::SetNextItemWidth(half_width * 0.25f);
        ImGui::InputText("##pin", pin_input, IM_ARRAYSIZE(pin_input));
        // Present the option to exit authenticated state
        // ask for the pin
        // if pin fails, launch error pop-up, reset permissions
    }
    else // is authenticated && is autorised
    {
        // Present core atm functionality
        // allow to exit from the menu (reset permissions)
    }

    ImGui::SetCursorPos(ImVec2(half_width, 0));
    ImGui::BeginChild("Top right child", ImVec2(half_width, half_height), true);
    ImGui::Text("This is the top right child window.");
    ImGui::EndChild();
    
    ImGui::SetCursorPos(ImVec2(half_width, half_height));
    ImGui::BeginChild("Bottom right child", ImVec2(half_width, half_height), true);
    ImGui::Text("This is the bottom right child window.");
    ImGui::EndChild();

    /* Frame render info */
    ImGui::SetCursorPos(ImVec2(10, io.DisplaySize.y - 50));
    ImGui::Text("Current FPS: %.0f", io.Framerate);

    ImGui::SetCursorPosX(10);
    ImGui::Text("Last frame time: %.4f", io.DeltaTime);

    ImGui::End();
}

int main()
{
    // Init
    Util::glfw_init(Config::GL_VERSION_MAJOR, Config::GL_VERSION_MINOR);
    GLFWwindow *const window = Util::window_init(Config::WIN_WIDTH, Config::WIN_HEIGHT, "ATM Frontend");
    Util::glfw_log_info();
    Util::imgui_init(window);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events
        glfwPollEvents();

        /* Handle all logic here (before rendering) */

        // Start the ImGui render frame
        Util::imgui_start_frame();

        // Push UI updates
        renderUI();

        // Render the UI
        Util::imgui_render_frame(window);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    Util::imgui_cleanup();
    Util::glfw_cleanup(window);
    return 0;
}
