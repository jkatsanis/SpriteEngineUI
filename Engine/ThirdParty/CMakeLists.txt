# ThirdParty/CMakeLists.txt

file(GLOB IMGUI_SOURCES
    ${EXTERNAL_LIBRARIES_FOLDER}/imgui/*.h
    ${EXTERNAL_LIBRARIES_FOLDER}/imgui/*.cpp
)

file(GLOB IMGUI_SFML_SOURCES 
    ${EXTERNAL_LIBRARIES_FOLDER}/imgui-sfml/*.h
    ${EXTERNAL_LIBRARIES_FOLDER}/imgui-sfml/*.cpp
)

file(GLOB UTILITY_SOURCES 
    ${EXTERNAL_LIBRARIES_FOLDER}/utility/dirent.h
    ${EXTERNAL_LIBRARIES_FOLDER}/utility/icons.h
    ${EXTERNAL_LIBRARIES_FOLDER}/utility/UtilityFunctions.cpp
    ${EXTERNAL_LIBRARIES_FOLDER}/utility/UtilityFunctions.h
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${IMGUI_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${IMGUI_SFML_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${UTILITY_SOURCES})

add_library(imgui STATIC ${IMGUI_SOURCES})
add_library(imgui_sfml STATIC ${IMGUI_SFML_SOURCES})
add_library(utility STATIC ${UTILITY_SOURCES})

set_target_properties(imgui PROPERTIES FOLDER "ThirdParty")
set_target_properties(imgui_sfml PROPERTIES FOLDER "ThirdParty")
set_target_properties(utility PROPERTIES FOLDER "ThirdParty")

include_directories("${EXTERNAL_LIBRARIES_FOLDER}/imgui")
include_directories("${EXTERNAL_LIBRARIES_FOLDER}/imgui-sfml")
include_directories("${EXTERNAL_LIBRARIES_FOLDER}/sfml/include")

if(MSVC)
    target_compile_options(imgui PUBLIC /W4)
    target_compile_options(imgui_sfml PUBLIC /W4)
    target_compile_options(utility PUBLIC /W4)
endif()