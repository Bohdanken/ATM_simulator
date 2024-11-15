#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "glfw_config.h"
#include "window_config.h"
#include "text_config.h"
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

    const float half_width = 0.5f * io.DisplaySize.x;
    const float half_height = 0.5f * io.DisplaySize.y;
    
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(Config::MAIN_WIN_TITLE, nullptr, Config::MAIN_WIN_FLAGS);

    if (!is_authenticated) // is not authenticated && is not authorised
    {
        static const ImVec2 out_text_size = ImGui::CalcTextSize(Config::ACCOUNT_NUMBER_LABEL);
        static const ImVec2 in_label_size = ImGui::CalcTextSize(Config::ACCOUNT_NUMBER_TEMPLATE);

        ImGui::SetCursorPos(ImVec2(
            half_width  - (0.5f * out_text_size.x),
            half_height - (0.5f * out_text_size.y)
        ));
        ImGui::SetNextItemWidth(out_text_size.x);
        ImGui::Text(Config::ACCOUNT_NUMBER_LABEL);

        ImGui::SetCursorPos(ImVec2(
            half_width  - (0.5f * in_label_size.x),
            half_height + (0.5f * in_label_size.y) + Config::Y_PADDING
        ));
        ImGui::SetNextItemWidth(in_label_size.x);
        ImGui::InputText(
            Config::ACCOUNT_NUMBER_INPUT_TITLE, 
            account_input, 
            IM_ARRAYSIZE(account_input), 
            ImGuiInputTextFlags_CharsDecimal
        );
    }
    else if (!is_authorised) // is authenticated, but is not authorised
    {
        // Present the option to exit authenticated state
        // ask for the pin
        // if pin fails, launch error pop-up, reset permissions

        static const ImVec2 out_text_size = ImGui::CalcTextSize(Config::PIN_LABEL);
        static const ImVec2 in_label_size = ImGui::CalcTextSize(Config::PIN_TEMPLATE);

        ImGui::SetCursorPos(ImVec2(
            half_width  - (0.5f * out_text_size.x),
            half_height - (0.5f * out_text_size.y)
        ));
        ImGui::SetNextItemWidth(out_text_size.x);
        ImGui::Text(Config::PIN_LABEL);

        ImGui::SetCursorPos(ImVec2(
            half_width  - (0.5f * in_label_size.x),
            half_height + (0.5f * in_label_size.y) + Config::Y_PADDING
        ));
        ImGui::SetNextItemWidth(in_label_size.x);
        ImGui::InputText(
            Config::PIN_INPUT_TITLE,
            pin_input,
            IM_ARRAYSIZE(pin_input),
            ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsDecimal
        );
    }
    else // is authenticated && is autorised
    {
        // Present core atm functionality
        // allow to exit from the menu (reset permissions)
    }

    /* ATM state and user session info */
    // TODO: Implement

    /*ImGui::SetCursorPos(ImVec2(half_width, 0));
    ImGui::BeginChild("Top right child", ImVec2(half_width, half_height), true);
    ImGui::Text("This is the top right child window.");
    ImGui::EndChild();*/
    
    /*ImGui::SetCursorPos(ImVec2(half_width, half_height));
    ImGui::BeginChild("Bottom right child", ImVec2(half_width, half_height), true);
    ImGui::Text("This is the bottom right child window.");
    ImGui::EndChild();*/

    /* Frame render info */
    ImGui::SetCursorPos(ImVec2(10, io.DisplaySize.y - 50));
    ImGui::Text("Current FPS: %.0f", io.Framerate);

    ImGui::SetCursorPosX(10);
    ImGui::Text("Last frame time: %.4f", io.DeltaTime);

    ImGui::End();
}

static void validate_input()
{
    // Will launch modal error dialog when validation fails
}

static void process_events()
{
    if (ImGui::IsKeyPressed(ImGuiKey_Enter) && ImGui::GetIO().WantCaptureKeyboard)
    {
        // TODO: perform input validation based on ATM and user session state
        std::cout << "The enter key was pressed!" << std::endl;
        is_authenticated = true;
    }
}

int main()
{
    // Init
    Util::glfw_init(Config::GL_VERSION_MAJOR, Config::GL_VERSION_MINOR);
    GLFWwindow *const window = Util::window_init(Config::WIN_WIDTH, Config::WIN_HEIGHT, Config::WIN_TITLE);
    Util::glfw_log_info();
    Util::imgui_init(window);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events
        glfwPollEvents();
        process_events();

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
