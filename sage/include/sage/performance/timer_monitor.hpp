#pragma once

#include <chrono>

namespace sage::performance
{
    // Interface for a monitor that can handle measurements
    class timer_monitor
    {
    public:
        virtual ~timer_monitor() = default;

        virtual void add_measurement(std::chrono::milliseconds duration_in_ms) = 0;
    };
}