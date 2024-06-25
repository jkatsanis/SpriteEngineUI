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

set(ENGINE_LIBRARY_DEBUG "${FILE_CONTENT}/Build/Engine/Debug/Engine.lib")
set(ENGINE_LIBRARY_RELEASE "${FILE_CONTENT}/Build/Engine/Release/Engine.lib")

set(ADDITIONAL_INCLUDE_PATHS
    Engine/Source
    Core
    Core/Source
    Core/Source/Include
    Core/Source/Include/SFML
    Core/Source/Include/ImGui
)

foreach(PATH ${ADDITIONAL_INCLUDE_PATHS})
    list(APPEND FULL_PATHS "${FILE_CONTENT}/${PATH}")
endforeach()

include_directories(${FULL_PATHS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Assets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine)

# Link libs
target_link_libraries(${PROJECT_FOLDER} PRIVATE
    opengl32.lib
    $<$<CONFIG:Debug>:${CORE_LIBRAY_DEBUG}>
    $<$<CONFIG:Release>:${CORE_LIBRAY_RELEASE}>

    $<$<CONFIG:Debug>:${ENGINE_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${ENGINE_LIBRARY_RELEASE}>

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

set(DEBUG_DIR ${CMAKE_BINARY_DIR}/Debug)
set(RELEASE_DIR ${CMAKE_BINARY_DIR}/Release)
set(PATH_DLL_LIBRARIES ${FILE_CONTENT}/Editor/Source/Include/DLL)

# Set debug directory and copy dlls
file(MAKE_DIRECTORY ${DEBUG_DIR})
file(MAKE_DIRECTORY ${RELEASE_DIR})

configure_file(${PATH_DLL_LIBRARIES}/sfml-audio-d-2.dll ${DEBUG_DIR} COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-graphics-d-2.dll ${DEBUG_DIR} COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-network-d-2.dll ${DEBUG_DIR} COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-system-d-2.dll ${DEBUG_DIR} COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-window-d-2.dll ${DEBUG_DIR} COPYONLY)

configure_file(${PATH_DLL_LIBRARIES}/sfml-audio-2.dll ${RELEASE_DIR} COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-graphics-2.dll ${RELEASE_DIR} COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-network-2.dll ${RELEASE_DIR} COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-system-2.dll ${RELEASE_DIR} COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-window-2.dll ${RELEASE_DIR} COPYONLY)

if(MSVC)
    target_compile_options(${PROJECT_FOLDER} PRIVATE /W4)
endif()