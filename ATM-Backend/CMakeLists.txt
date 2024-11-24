cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)


# Set project name
project("ATM_simulator")

set(CMAKE_TOOLCHAIN_FILE "conan_toolchain.cmake" CACHE STRING "Toolchain file")

find_package(libpqxx)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Include directories: ${CMAKE_INCLUDE_PATH}")

file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/repositories/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DTO/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Services/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
    # Add other directories as needed
)

add_executable(TestsProject ${SOURCE_FILES} "main.cpp")

# Link the libraries provided by Conan
target_link_libraries(TestsProject PRIVATE  libpqxx::pqxx)

