cmake_minimum_required(VERSION 3.10)
project(SpriteEngine)

set(CORE_SOURCE_FOLDER_NAME Source)
set(EDITOR_NAME Editor)

set(EXTERNAL_LIBRARIES_FOLDER ${CMAKE_SOURCE_DIR}/ThirdParty)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the subprojects
add_subdirectory(Engine)
add_subdirectory(Editor)

# The gamegnine the user uses eventually
add_subdirectory(Framework)

# Thirdparty
add_subdirectory(ThirdParty/SFML)
add_subdirectory(ThirdParty)
