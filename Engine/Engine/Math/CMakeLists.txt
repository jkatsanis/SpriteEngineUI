# Math/CMakeLists.txt

file(GLOB_RECURSE MATH_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MATH_SOURCE_FILES})

# Create the Core library and add the source files to it
add_library(Math STATIC ${MATH_SOURCE_FILES})

set_target_properties(Math PROPERTIES FOLDER "Engine")

if(MSVC)
    target_compile_options(Math PUBLIC /W4)
endif()