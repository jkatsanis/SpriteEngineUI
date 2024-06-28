# Engine/Core/Renderer3D/CMakeLists.txt

file(GLOB_RECURSE RENDERER3D_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RENDERER3D_SOURCE_FILES})

add_library(Renderer3D STATIC ${RENDERER3D_SOURCE_FILES})

set_target_properties(Renderer3D PROPERTIES FOLDER "Engine")

if(MSVC)
    target_compile_options(Renderer3D PUBLIC /W4)
endif()