# Core/CMakeLists.txt

file(GLOB_RECURSE CORE_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCE_FILES})

# Create the Core library and add the source files to it
add_library(Core STATIC ${CORE_SOURCE_FILES})

target_link_libraries(Core PUBLIC
    opengl32.lib
    $<$<CONFIG:Debug>:${EXTERNAL_LIBRARIES_FOLDER}/sfml-audio-d.lib>
    $<$<CONFIG:Debug>:${EXTERNAL_LIBRARIES_FOLDER}/sfml-graphics-d.lib>
    $<$<CONFIG:Debug>:${EXTERNAL_LIBRARIES_FOLDER}/sfml-network-d.lib>
    $<$<CONFIG:Debug>:${EXTERNAL_LIBRARIES_FOLDER}/sfml-system-d.lib>
    $<$<CONFIG:Debug>:${EXTERNAL_LIBRARIES_FOLDER}/sfml-window-d.lib>

    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-audio.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-graphics.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-network.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-system.lib>
    $<$<CONFIG:Release>:${PATH_LIBRARIES}/sfml-window.lib>
)

if(MSVC)
    target_compile_options(Core PUBLIC /W4)
endif()