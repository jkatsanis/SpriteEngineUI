# Core/CMakeLists.txt

file(GLOB_RECURSE CORE_SOURCE_FILES
    "Source/*.h"
    "Source/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCE_FILES})

# Create the Core library and add the source files to it
add_library(Core STATIC ${CORE_SOURCE_FILES})

target_include_directories(Core PUBLIC "Source")

target_include_directories(Core PUBLIC "${EXTERNAL_LIBRARIES_FOLDER}/imgui")
target_include_directories(Core PUBLIC "${EXTERNAL_LIBRARIES_FOLDER}/imgui-sfml")
 
target_include_directories(Core PUBLIC ${EXTERNAL_LIBRARIES_FOLDER})

set(SFML_DIR "${EXTERNAL_LIBRARIES_FOLDER}/sfml")
add_subdirectory(${SFML_DIR} "${CMAKE_BINARY_DIR}/sfml_build")
target_include_directories(Core PUBLIC "${SFML_DIR}/include")
target_link_libraries(Core PUBLIC sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

find_package(OpenGL REQUIRED)
target_link_libraries(Core PRIVATE OpenGL::GL)

if(MSVC)
    target_compile_options(Core PUBLIC /W4)
endif()