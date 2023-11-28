# Template Solution

cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${PROJECT_FOLDER})
set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE GAME_SOURCES
    "Engine/Source/*.cpp"
    "Engine/Source/*.h"
    "Assets/*.h"
    "Assets/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GAME_SOURCES})

add_executable(${PROJECT_FOLDER} ${GAME_SOURCES})

file(READ "Engine\\Saves\\enginepath.txt" FILE_CONTENT)
file(TO_CMAKE_PATH "${FILE_CONTENT}" FILE_CONTENT)
string(STRIP "${FILE_CONTENT}" FILE_CONTENT)
set(PATH_LIBRARIES "${FILE_CONTENT}/Core/Source/Include/LIB")


set(CORE_LIBRAY_DEBUG "${FILE_CONTENT}/Build/Core/Debug/Core.lib")
set(CORE_LIBRAY_RELEASE "${FILE_CONTENT}/Build/Core/Release/Core.lib")


set(ADDITIONAL_INCLUDE_PATHS
    Core
    Core/Source
    Core/Source/Include
    Core/Source/Include/SFML
    Core/Source/Includ/ImGui
)

foreach(PATH ${ADDITIONAL_INCLUDE_PATHS})
    list(APPEND FULL_PATHS "${FILE_CONTENT}/${PATH}")
endforeach()

include_directories(${FULL_PATHS})
include_directories(Assets)

# Link libs
target_link_libraries(${PROJECT_FOLDER} PRIVATE
    opengl32.lib
    $<$<CONFIG:Debug>:${CORE_LIBRAY_DEBUG}>
    $<$<CONFIG:Release>:${CORE_LIBRAY_RELEASE}>

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