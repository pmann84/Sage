#pragma once

#include "timer_monitor.hpp"

#include <chrono>
#include <iostream>
#include <functional>

namespace sage::performance
{
    // RAII Timer
    /// NOTE: Does this measure the destructor calls of other objects in the timer scope?
    /// Destructors are called in reverse order to which they were initialised, the last
    /// constructed object gets destructed first, same as in stack unwinding when exception
    /// is thrown so where you place your timing is important.
    class timer
    {
    public:
        using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

        explicit timer(timer_monitor &monitor) : m_monitor(monitor)
        {
            m_start_time_point = std::chrono::high_resolution_clock::now();
        }
        ~timer()
        {
            stop();
        }

    private:
        void stop() const
        {
            // Calculate time and return it to the monitor
            const auto end_time_point = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_point - m_start_time_point);
            // Notify the monitor
            m_monitor.add_measurement(duration);
        }

    private:
        // start time point
        time_point_t m_start_time_point;
        // results monitor
        timer_monitor &m_monitor;
    };

    // Convenience calling function
    inline void measure(timer_monitor &monitor, const std::function<void()> &func)
    {
        timer t(monitor);
        func();
    }
}