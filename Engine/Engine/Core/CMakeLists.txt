# Core/CMakeLists.txt

file(GLOB_RECURSE CORE_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCE_FILES})

set(SFML_DIR "${EXTERNAL_LIBRARIES_FOLDER}/sfml")

# Create the Core library and add the source files to it
add_library(Core STATIC ${CORE_SOURCE_FILES})

target_include_directories(Core PUBLIC "Source")
target_include_directories(Core PUBLIC "${EXTERNAL_LIBRARIES_FOLDER}/imgui")
target_include_directories(Core PUBLIC "${EXTERNAL_LIBRARIES_FOLDER}/imgui-sfml")
target_include_directories(Core PUBLIC "${EXTERNAL_LIBRARIES_FOLDER}/utility")
target_include_directories(Core PUBLIC ${EXTERNAL_LIBRARIES_FOLDER})
target_include_directories(Core PUBLIC "${SFML_DIR}/include")
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/Engine)

add_subdirectory(${SFML_DIR} "${CMAKE_BINARY_DIR}/sfml-build/")

target_link_libraries(Core PUBLIC 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio 
    sfml-network 
    utility 
    imgui_sfml
    imgui
    opengl32.lib
    Math
)

set_target_properties(Core PROPERTIES FOLDER "Engine")

if(MSVC)
    target_compile_options(Core PUBLIC /W4)
endif()