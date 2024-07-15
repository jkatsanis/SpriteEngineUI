# Core/CMakeLists.txt

file(GLOB_RECURSE ENGINE_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCE_FILES})

# Create the Core library and add the source files to it
add_library(Framework STATIC ${ENGINE_SOURCE_FILES})

target_link_libraries(Framework PUBLIC 
    Core
)

if(MSVC)
    target_compile_options(Framework PUBLIC /W4)
endif()
