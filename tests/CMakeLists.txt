find_package(GTest REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test-bin)

include(GoogleTest)

add_executable(test_logger
    test_logger.cpp
)

target_link_libraries(test_logger
    PRIVATE
        GTest::gmock_main
)

gtest_discover_tests(test_logger)
