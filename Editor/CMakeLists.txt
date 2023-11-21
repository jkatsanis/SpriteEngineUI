# Editor/CMakeLists.txt


set(EDITOR_NAME Editor)
set(LIB_FOLDER_NAME LIB)
set(DLL_FOLDER_NAME DLL)

set(PATH_LIBRARIES ${CMAKE_SOURCE_DIR}/${EDITOR_NAME}/Source/Include)

# Define the executable
add_executable(${EDITOR_NAME} 
    Editor.cpp)

# Link the Core library
target_link_libraries(${EDITOR_NAME} PRIVATE Core)

# Link SFML libraries using keyword signature
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