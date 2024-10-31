cmake_minimum_required(VERSION 3.8)

# Set project name
project("ATM_simulator")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Conan build info
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Add the executable for TestsProject
add_executable(TestsProject "main.cpp" "AccountTest.h" "AccountTest.cpp" "CardTest.h" "utils/assert_utils.h")

# Link the libraries provided by Conan
target_link_libraries(TestsProject ${CONAN_LIBS})

# TODO: Add tests and install targets if needed.
