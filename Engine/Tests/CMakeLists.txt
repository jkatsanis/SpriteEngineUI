set(Editor_Test Editor_Test)
set(Core_Test Core_Test)

add_subdirectory(${Editor_Test})
add_subdirectory(${Core_Test})

set_target_properties(${Editor_Test} PROPERTIES FOLDER "Tests")
set_target_properties(${Core_Test} PROPERTIES FOLDER "Tests")