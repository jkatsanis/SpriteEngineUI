# Template Solution
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
set(CMAKE_BUILD_TYPE Debug) # CHANGE WHEN BUILDING IN RELEASE TO Release
# !!!!!!!!!!!!!!!

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

set(ADDITIONAL_INCLUDE_PATHS
    Engine/Core/Source
    Framework/Source
    Engine
    ThirdParty
    ThirdParty/imgui
    ThirdParty/utility
    ThirdParty/imgui-sfml
)

foreach(PATH ${ADDITIONAL_INCLUDE_PATHS})
    list(APPEND FULL_PATHS "${FILE_CONTENT}/${PATH}")
endforeach()

include_directories(${FULL_PATHS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Assets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine)

# Link libs
target_link_libraries(${PROJECT_FOLDER} PRIVATE
    Framework
)

if(MSVC)
    target_compile_options(${PROJECT_FOLDER} PRIVATE /W4)
endif()

add_subdirectory(${FILE_CONTENT}/Engine Engine)
add_subdirectory(${FILE_CONTENT}/ThirdParty/sfml ThirdParty/sfml)
add_subdirectory(${FILE_CONTENT}/ThirdParty ThirdParty)
add_subdirectory(${FILE_CONTENT}/Framework Framework)

set(SFML_DLL_DIR "${CMAKE_BINARY_DIR}/ThirdParty/sfml/lib")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${PROJECT_FOLDER} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Debug/sfml-audio-d-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Debug/sfml-graphics-d-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Debug/sfml-network-d-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Debug/sfml-system-d-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Debug/sfml-window-d-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET ${PROJECT_FOLDER} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Release/sfml-audio-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Release/sfml-graphics-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Release/sfml-network-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Release/sfml-system-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLL_DIR}/Release/sfml-window-2.dll $<TARGET_FILE_DIR:${PROJECT_FOLDER}>
    )
endif()