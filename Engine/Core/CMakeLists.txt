# Core/CMakeLists.txt

file(GLOB_RECURSE CORE_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCE_FILES})

# Create the Core library and add the source files to it
add_library(Core STATIC ${CORE_SOURCE_FILES})

# Include directories if needed
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})