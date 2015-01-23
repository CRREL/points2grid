configure_file(
    config.hpp.in
    ${PROJECT_BINARY_DIR}/test/config.hpp
    )

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/test
    ${gtest_SOURCE_DIR}/include
    )

set(src
    interpolation_test.cpp
    )

set(deps
    ${P2G_LIB_NAME}
    gtest_main
    )

add_executable(${P2G_TEST_NAME} ${src})
target_link_libraries(${P2G_TEST_NAME} ${deps})
add_test(unit-tests ${P2G_TEST_NAME})
