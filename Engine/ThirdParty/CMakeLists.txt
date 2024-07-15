# ThirdParty/CMakeLists.txt

file(GLOB IMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp
)

file(GLOB IMGUI_SFML_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml/*.cpp
)

file(GLOB UTILITY_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/dirent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/icons.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/UtilityFunctions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/UtilityFunctions.h
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

# SFML
set_target_properties(sfml-window PROPERTIES FOLDER "ThirdParty/SFML")
set_target_properties(sfml-audio PROPERTIES FOLDER "ThirdParty/SFML")
set_target_properties(sfml-graphics PROPERTIES FOLDER "ThirdParty/SFML")
set_target_properties(sfml-main PROPERTIES FOLDER "ThirdParty/SFML")
set_target_properties(sfml-network PROPERTIES FOLDER "ThirdParty/SFML")
set_target_properties(sfml-system PROPERTIES FOLDER "ThirdParty/SFML")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/imgui")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/sfml/include")

if(MSVC)
    target_compile_options(imgui PUBLIC /W4)
    target_compile_options(imgui_sfml PUBLIC /W4)
    target_compile_options(utility PUBLIC /W4)
endif()