# Core/CMakeLists.txt

# Define the library
add_library(Core STATIC
    Source/Core.h
    Source/Core.cpp)

# Include directories if needed
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})