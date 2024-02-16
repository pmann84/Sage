set(PROJECT_NAME "sage")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(
        ${PROJECT_NAME}
        INTERFACE
        "include/sage.hpp"
        "include/sage/argparse/argparse.hpp"
        "include/sage/string/utilities.hpp"
        "include/sage/performance/timer.hpp"
        "include/sage/performance/timer_monitor.hpp"
        "include/sage/performance/monitors.hpp"
)