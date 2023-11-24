# Template Solution

cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${PROJECT_FOLDER})
set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE GAME_SOURCES
    "Source/*.cpp"
    "Source/*.h"
)

add_executable(${PROJECT_FOLDER} ${GAME_SOURCES})

file(READ "Engine\\Saves\\enginepath.txt" FILE_CONTENT)
file(TO_CMAKE_PATH "${FILE_CONTENT}" FILE_CONTENT)
string(STRIP "${FILE_CONTENT}" FILE_CONTENT)
set(PATH_LIBRARIES "${FILE_CONTENT}/Core/Source/Include/LIB")

message("File content: ${FILE_CONTENT}")


set(ADDITIONAL_INCLUDE_PATHS
    Core
    Core/Source
    Core/Source/Include
    Core/Source/Include/SFML
    Core/Source/Includ/ImGui
)

foreach(PATH ${ADDITIONAL_INCLUDE_PATHS})
    list(APPEND FULL_PATHS "${FILE_CONTENT}/${PATH}")
    message(${FULL_PATHS})
endforeach()

include_directories(${FULL_PATHS})

# Link libs
target_link_libraries(${PROJECT_FOLDER} PRIVATE
    opengl32.lib
    $<$<CONFIG:Debug>:C:/Dev/C++/SpriteEngineUI/Engine/Build/Core/Debug/Core.lib>

    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-audio-d.lib>
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-graphics-d.lib>
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-network-d.lib>
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-system-d.lib>
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-window-d.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-audio.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-graphics.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-network.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-system.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-window.lib>
)

