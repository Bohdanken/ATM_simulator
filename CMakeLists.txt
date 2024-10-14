cmake_minimum_required(VERSION 3.8)

# Set project name
project("ATM_simulator")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set Boost root directory explicitly (Adjust according to your system)
set(BOOST_ROOT "C:/local/boost_1_86_0")
set(Boost_INCLUDE_DIR "C:/local/boost_1_86_0")
set(Boost_LIBRARY_DIR "C:/local/boost_1_86_0/lib64-msvc-14.3")

# Find Boost (Make sure it finds system and thread components)
find_package(Boost REQUIRED COMPONENTS system thread)

# If Boost is found, include its directories and link libraries
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    # Add the executable for TestsProject
    add_executable(TestsProject "main.cpp" "AccountTest.h" "AccountTest.cpp" "CardTest.h" "utils/assert_utils.h")

    # Link Boost libraries
    target_link_libraries(TestsProject ${Boost_LIBRARIES})

else()
    message(FATAL_ERROR "Boost not found, please check your installation")
endif()

# Additional settings for MSVC Debug information (optional, can be adjusted)
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET TestsProject PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
