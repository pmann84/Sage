cmake_minimum_required(VERSION 3.17)

project(test_sage)

include(GoogleTest)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS test_*/*.cpp test_*/*.hpp)

message(${TEST_SOURCES})
add_executable(${PROJECT_NAME} ${TEST_SOURCES}
        test_performance/monitors_tests.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC sage)

target_link_libraries(${PROJECT_NAME} PUBLIC gtest_main)
target_link_libraries(${PROJECT_NAME} PUBLIC gmock)
target_link_libraries(${PROJECT_NAME} PUBLIC gmock_main)
target_link_libraries(${PROJECT_NAME} PUBLIC gtest)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/test_argparse/test_config_files $<TARGET_FILE_DIR:test_sage>/)

gtest_discover_tests(${PROJECT_NAME})