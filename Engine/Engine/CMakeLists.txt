# Core/CMakeLists.txt

file(GLOB_RECURSE ENGINE_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCE_FILES})

# Create the Core library and add the source files to it
add_library(${ENGINE_NAME} STATIC ${ENGINE_SOURCE_FILES})

target_link_libraries(Engine PUBLIC
    Core
)

if(MSVC)
    target_compile_options(${ENGINE_NAME} PUBLIC /W4)
endif()
