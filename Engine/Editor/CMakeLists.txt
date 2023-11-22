# Editor/CMakeLists.txt

# Define the executable

file(GLOB_RECURSE EDITOR_SOURCE_FILES
    "Source/*.cpp"
    "Source/*.h"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCE_FILES})

add_executable(${EDITOR_NAME} ${EDITOR_SOURCE_FILES})

# Link libraries
target_link_libraries(${EDITOR_NAME} PRIVATE Core)
target_link_libraries(${EDITOR_NAME} PRIVATE
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-audio-d.lib
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-graphics-d.lib
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-network-d.lib
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-system-d.lib
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-window-d.lib
)

# Set debug directory and copy dlls
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Editor/Debug)

configure_file(${PATH_LIBRARIES}/${DLL_FOLDER_NAME}/sfml-audio-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)
configure_file(${PATH_LIBRARIES}/${DLL_FOLDER_NAME}/sfml-graphics-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)
configure_file(${PATH_LIBRARIES}/${DLL_FOLDER_NAME}/sfml-network-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)
configure_file(${PATH_LIBRARIES}/${DLL_FOLDER_NAME}/sfml-system-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)
configure_file(${PATH_LIBRARIES}/${DLL_FOLDER_NAME}/sfml-window-d-2.dll ${CMAKE_BINARY_DIR}/${EDITOR_NAME}/Debug COPYONLY)

# TODO Make release dir