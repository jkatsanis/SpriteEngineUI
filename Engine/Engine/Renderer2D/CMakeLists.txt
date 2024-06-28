# Engine/Core/Renderer2D/CMakeLists.txt

file(GLOB_RECURSE RENDERER2D_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RENDERER2D_SOURCE_FILES})

add_library(Renderer2D STATIC ${RENDERER2D_SOURCE_FILES})

set_target_properties(Renderer2D PROPERTIES FOLDER "Engine")

if(MSVC)
    target_compile_options(Renderer2D PUBLIC /W4)
endif()