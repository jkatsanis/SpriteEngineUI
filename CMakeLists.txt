cmake_minimum_required(VERSION 3.10)
project(SpriteEngine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include SFML
include_directories($(CMAKE_SOURCE_DIR)/Core/Source/Include)

# Add the subprojects
add_subdirectory(Core)
add_subdirectory(Editor)

