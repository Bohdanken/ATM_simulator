# app/util/CMakeLists.txt

# N.B. Swap out 'STATIC' for 'INTERFACE' to get a header only library (like config)
add_library(
    AppUtil STATIC 
    "impl/glfw_util.cpp"
    "impl/imgui_util.cpp"
)

# ...and swap out 'PUBLIC' for 'INTERFACE' for a header only library
target_include_directories(AppUtil PUBLIC headers)

# Linking glfw with further propagation
target_link_libraries(AppUtil PUBLIC glfw)
