# Framework/CMakeLists.txt
# The gamengine lib contains functions like AddForce

file(GLOB_RECURSE ENGINE_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCE_FILES})

add_library(Framework STATIC ${ENGINE_SOURCE_FILES})

target_link_libraries(Framework PUBLIC 
    Core
)
set_target_properties(Framework PROPERTIES FOLDER "Engine")

if(MSVC)
    target_compile_options(Framework PUBLIC /W4)
endif()
