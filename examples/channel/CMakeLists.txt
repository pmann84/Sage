set(PROJECT_NAME "sage_channel_example")

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} sage)

target_sources(
        ${PROJECT_NAME}
        PRIVATE 
        channel_example.cpp
)