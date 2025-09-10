set(PROJECT_NAME "sage_argparse_example")

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} sage)

target_sources(
        ${PROJECT_NAME}
        PRIVATE
        argparse_example.cpp
)