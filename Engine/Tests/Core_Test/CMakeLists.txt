# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
set(CMAKE_BUILD_TYPE Debug) # CHANGE WHEN BUILDING IN RELEASE TO Release
# !!!!!!!!!!!!!!!

project(${Core_Test})

file(GLOB_RECURSE CORE_TEST_SRC_FILES
    "*.cpp"
    "*.h"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_TEST_SRC_FILES})

add_executable(${Core_Test}
    ${CORE_TEST_SRC_FILES}
)

add_test(NAME CoreTests COMMAND ${Core_Test})

target_link_libraries(${Core_Test} PRIVATE
  GTest::gtest
  GTest::gtest_main
  Core
  Framework
)

include_directories(${CMAKE_SOURCE_DIR}/Framework/Source)

set(SFML_DLL_DIR "${CMAKE_BINARY_DIR}/ThirdParty/sfml/lib")

if(MSVC)
    # Custom command to copy DLLs based on build configuration
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET Core_Test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Debug/sfml-audio-d-2.dll $<TARGET_FILE_DIR:Core_Test>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Debug/sfml-graphics-d-2.dll $<TARGET_FILE_DIR:Core_Test>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Debug/sfml-network-d-2.dll $<TARGET_FILE_DIR:Core_Test>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Debug/sfml-system-d-2.dll $<TARGET_FILE_DIR:Core_Test>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Debug/sfml-window-d-2.dll $<TARGET_FILE_DIR:Core_Test>
        )
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(TARGET Core_Test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Release/sfml-audio-2.dll $<TARGET_FILE_DIR:Core_Test>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Release/sfml-graphics-2.dll $<TARGET_FILE_DIR:Core_Test>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Release/sfml-network-2.dll $<TARGET_FILE_DIR:Core_Test>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Release/sfml-system-2.dll $<TARGET_FILE_DIR:Core_Test>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_DLL_DIR}/Release/sfml-window-2.dll $<TARGET_FILE_DIR:Core_Test>
        )
    endif()
endif()
if(MSVC)
    target_compile_options(Core_Test PRIVATE /W4)
    add_compile_options(/MP)
endif()
