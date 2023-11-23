cmake_minimum_required(VERSION 3.10)
project(SpriteEngine)

set(CORE_SOURCE_FOLDER_NAME Source)
set(EDITOR_NAME Editor)

set(PATH_LIBRARIES ${CMAKE_SOURCE_DIR}/Core/Source/Include/LIB)
set(PATH_DLL_LIBRARIES ${CMAKE_SOURCE_DIR}/Editor/Source/Include/DLL)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include SFML
include_directories($(CMAKE_SOURCE_DIR)/Core/${CORE_SOURCE_FOLDER_NAME}/Sprite)
include_directories($(CMAKE_SOURCE_DIR)/Core/${CORE_SOURCE_FOLDER_NAME}/Include/ImGui)
include_directories($(CMAKE_SOURCE_DIR)/Core/${CORE_SOURCE_FOLDER_NAME}/Include)
include_directories($(CMAKE_SOURCE_DIR)/Core/${CORE_SOURCE_FOLDER_NAME})


# Add the subprojects
add_subdirectory(Core)
add_subdirectory(Editor)
