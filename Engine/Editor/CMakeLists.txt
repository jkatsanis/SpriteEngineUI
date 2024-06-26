# Editor/CMakeLists.txt

# Define the executable

file(GLOB_RECURSE EDITOR_SOURCE_FILES
    "Source/*.cpp"
    "Source/*.h"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCE_FILES})


add_executable(${EDITOR_NAME}
 ${EDITOR_SOURCE_FILES}
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Link libraries
target_link_libraries(${EDITOR_NAME} PRIVATE
    Core
)

target_include_directories(Core PRIVATE "${EXTERNAL_LIBRARIES_FOLDER}/imgui")

if(MSVC)
    target_compile_options(${EDITOR_NAME} PRIVATE /W4)
endif()


