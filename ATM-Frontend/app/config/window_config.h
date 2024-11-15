#pragma once
#ifndef WINDOW_CONFIG_H
#define WINDOW_CONFIG_H

#include <imgui.h>

#include <cstdint>

namespace ATM::Config
{
    constexpr const char *WIN_TITLE = "ATM";

    /* Currently being discarded due to ImGuiWindowFlags_NoTitleBar in MAIN_WIN_FLAGS */
    constexpr const char *MAIN_WIN_TITLE = "ATM Main";

    /* Window default size (~16:9) */
    constexpr int32_t WIN_WIDTH = 1066;
    constexpr int32_t WIN_HEIGHT = 600;

    /* Window min size (~16:9) */
    constexpr int32_t MIN_WIN_WIDTH = 533;
    constexpr int32_t MIN_WIN_HEIGHT = 300;

    /* Window clear colour */
    constexpr struct
    {
        const float RED   = 0.125f;
        const float GREEN = 0.125f;
        const float BLUE  = 0.125f;
        const float ALPHA = 0.850f;
    } WIN_CLEAR;

    constexpr ImGuiWindowFlags MAIN_WIN_FLAGS =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
}

#endif // !WINDOW_CONFIG_H
