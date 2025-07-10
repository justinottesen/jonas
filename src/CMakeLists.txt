add_executable(jonas
    main.cpp
)

set_target_properties(jonas PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_compile_options(jonas PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

