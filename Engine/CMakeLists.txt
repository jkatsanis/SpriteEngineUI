cmake_minimum_required(VERSION 3.10)
project(SpriteEngine)

enable_testing()

set(CORE_SOURCE_FOLDER_NAME Source)
set(EDITOR_NAME Editor)

set(EXTERNAL_LIBRARIES_FOLDER ${CMAKE_SOURCE_DIR}/ThirdParty)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Add the subprojects
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Tests)

# Thirdparty
add_subdirectory(ThirdParty)
add_subdirectory(Framework)
