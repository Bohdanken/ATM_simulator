# app/util/CMakeLists.txt

add_library(
    AppUtil STATIC 
    "impl/glfw_util.cpp"
    "impl/imgui_util.cpp"
)
target_include_directories(AppUtil PUBLIC headers)

# Linking glfw with further propagation
target_link_libraries(AppUtil PUBLIC glfw)
target_link_libraries(AppUtil PRIVATE AppConfig)
