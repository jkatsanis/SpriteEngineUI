# Editor/CMakeLists.txt

# Define the executable

file(GLOB_RECURSE EDITOR_SOURCE_FILES
    "Source/*.cpp"
    "Source/*.h"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCE_FILES})

add_executable(${EDITOR_NAME} ${EDITOR_SOURCE_FILES})

include_directories($(CMAKE_CURRENT_SOURCE_DIR)/Source)

# Link libraries
target_link_libraries(${EDITOR_NAME} PRIVATE
    opengl32.lib
    Core
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-audio-d.lib>
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-graphics-d.lib>
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-network-d.lib>
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-system-d.lib>
    $<$<CONFIG:Debug>:${PATH_LIBRARIES}/sfml-window-d.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-audio.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/$sfml-graphics.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/$sfml-network.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-system.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-window.lib>
)

# Set debug directory and copy dlls
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Editor/Debug)

configure_file(${PATH_DLL_LIBRARIES}/sfml-audio-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-graphics-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-network-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-system-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-window-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Editor/Release)

configure_file(${PATH_DLL_LIBRARIES}/sfml-audio-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Release COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-graphics-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Release COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-network-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Release COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-system-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Release COPYONLY)
configure_file(${PATH_DLL_LIBRARIES}/sfml-window-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Release COPYONLY)

