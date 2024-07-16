# Math/CMakeLists.txt

file(GLOB_RECURSE MATH_SOURCE_FILES
    "*.h"
    "*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MATH_SOURCE_FILES})

add_library(Math STATIC ${MATH_SOURCE_FILES})

target_include_directories(Math PRIVATE "${EXTERNAL_LIBRARIES_FOLDER}/imgui")

target_link_libraries(Math PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio 
    sfml-network 
    imgui
)

set_target_properties(Math PROPERTIES FOLDER "Engine")

if(MSVC)
    target_compile_options(Math PUBLIC /W4)
endif()