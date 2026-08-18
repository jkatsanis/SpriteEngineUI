project(${Editor_Test})


add_executable(
  ${Editor_Test}
  hello_test.cc
)

add_test(NAME EditorTests COMMAND ${Editor_Test})

target_link_libraries(${Editor_Test} PRIVATE
  GTest::gtest
  GTest::gtest_main
  Core
)

# Include editor dirs
include_directories(${CMAKE_SOURCE_DIR}/Editor/Source)
