cmake_minimum_required(VERSION 3.10)
project(SpriteEngine)

set(EDITOR_NAME Editor)
set(TEMPLATE_NAME Template)
set(LIB_FOLDER_NAME LIB)
set(DLL_FOLDER_NAME DLL)

set(PATH_LIBRARIES ${CMAKE_SOURCE_DIR}/${EDITOR_NAME}/Source/Include)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include SFML
include_directories($(CMAKE_SOURCE_DIR)/Core/Source/Include/ImGui)
include_directories($(CMAKE_SOURCE_DIR)/Core/Source/Include)
include_directories($(CMAKE_SOURCE_DIR)/Core/Source)

# Add the subprojects
add_subdirectory(Core)
add_subdirectory(Editor)

# Adding this template here, so you can edit it in the vs studio sln, but when you createa  new project a new 
# .sln will be created and this template will be linked to the Core librarby
add_subdirectory(Template)

