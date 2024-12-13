﻿# CMakeList.txt : CMake project for ATM-Frontend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ATM-Frontend")

# Enforce C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths to dependency libraries
set(IMGUI_DIR "vendor/imgui")
set(GLFW_DIR "vendor/glfw")

# Add ImGui source files
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include directories
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

# Add GLFW
add_subdirectory(${GLFW_DIR} ${CMAKE_BINARY_DIR}/glfw_build)

# Add frontend app
add_subdirectory(app)

# Find required packages
find_package(OpenGL REQUIRED)

# Add source to this project's executable.
add_executable (ATM-Frontend "main.cpp" ${IMGUI_SOURCES})

# Link libraries
target_link_libraries(
    ATM-Frontend PRIVATE
    ${OPENGL_LIBRARIES}
    AppConfig
    AppUtil
)
