# Template/CMakeLists.txt

set(DLL_LIBRARIES_TEMPLATE ${CMAKE_SOURCE_DIR}/${TEMPLATE_NAME}/Source/Include)


file(GLOB_RECURSE TEMPLATE_SOURCE_FILES
    "Source/*.cpp"
    "Source/*.h"
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TEMPLATE_SOURCE_FILES})

add_executable(${TEMPLATE_NAME} ${TEMPLATE_SOURCE_FILES})

# Link the Core library
target_link_libraries(${TEMPLATE_NAME} PRIVATE Core)

# Link SFML libraries using keyword signature
target_link_libraries(${TEMPLATE_NAME} PRIVATE
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-audio-d.lib
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-graphics-d.lib
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-network-d.lib
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-system-d.lib
    ${PATH_LIBRARIES}/${LIB_FOLDER_NAME}/sfml-window-d.lib
)

# Set debug directory and copy dlls

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${TEMPLATE_NAME}/Debug)

configure_file(${DLL_LIBRARIES_TEMPLATE}/${DLL_FOLDER_NAME}/sfml-audio-d-2.dll ${CMAKE_BINARY_DIR}/${TEMPLATE_NAME}/Debug COPYONLY)
configure_file(${DLL_LIBRARIES_TEMPLATE}/${DLL_FOLDER_NAME}/sfml-graphics-d-2.dll ${CMAKE_BINARY_DIR}/${TEMPLATE_NAME}/Debug COPYONLY)
configure_file(${DLL_LIBRARIES_TEMPLATE}/${DLL_FOLDER_NAME}/sfml-network-d-2.dll ${CMAKE_BINARY_DIR}/${TEMPLATE_NAME}/Debug COPYONLY)
configure_file(${DLL_LIBRARIES_TEMPLATE}/${DLL_FOLDER_NAME}/sfml-system-d-2.dll ${CMAKE_BINARY_DIR}/${TEMPLATE_NAME}/Debug COPYONLY)
configure_file(${DLL_LIBRARIES_TEMPLATE}/${DLL_FOLDER_NAME}/sfml-window-d-2.dll ${CMAKE_BINARY_DIR}/${TEMPLATE_NAME}/Debug COPYONLY)

# TODO Make release dir