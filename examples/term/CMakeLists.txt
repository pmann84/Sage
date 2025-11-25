set(PROJECT_NAME "sage_term_colours_example")

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} sage)

target_sources(
        ${PROJECT_NAME}
        PRIVATE
        term_colours_example.cpp
)