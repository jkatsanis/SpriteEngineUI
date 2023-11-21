# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME_VAR SpriteEngine)
set(LIB_VAR LIB)
set(DLL_VAR DLL)

# Set the project name and version
project(${PROJECT_NAME_VAR} VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add an executable target for the main.cpp file
add_executable(${PROJECT_NAME_VAR}
    Source/main.cpp)

target_link_libraries(${PROJECT_NAME_VAR} ${CMAKE_SOURCE_DIR}/Source/Include/${LIB_VAR}/sfml-audio-d.lib)
target_link_libraries(${PROJECT_NAME_VAR} ${CMAKE_SOURCE_DIR}/Source/Include/${LIB_VAR}/sfml-graphics-d.lib)
target_link_libraries(${PROJECT_NAME_VAR} ${CMAKE_SOURCE_DIR}/Source/Include/${LIB_VAR}/sfml-network-d.lib)
target_link_libraries(${PROJECT_NAME_VAR} ${CMAKE_SOURCE_DIR}/Source/Include/${LIB_VAR}/sfml-system-d.lib)
target_link_libraries(${PROJECT_NAME_VAR} ${CMAKE_SOURCE_DIR}/Source/Include/${LIB_VAR}/sfml-window-d.lib)

include_directories($(CMAKE_SOURCE_DIR)/Source/Include)

set(CMAKE_BUILD_TYPE Debug)

# Set the default startup project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME_VAR})

# Create the Debug directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)

# Copy SFML DLLs to the Debug folder
configure_file(${CMAKE_SOURCE_DIR}/Source/Include/${DLL_VAR}/sfml-audio-d-2.dll ${CMAKE_BINARY_DIR}/Debug COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Source/Include/${DLL_VAR}/sfml-graphics-d-2.dll ${CMAKE_BINARY_DIR}/Debug COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Source/Include/${DLL_VAR}/sfml-network-d-2.dll ${CMAKE_BINARY_DIR}/Debug COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Source/Include/${DLL_VAR}/sfml-system-d-2.dll ${CMAKE_BINARY_DIR}/Debug COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Source/Include/${DLL_VAR}/sfml-window-d-2.dll ${CMAKE_BINARY_DIR}/Debug COPYONLY)