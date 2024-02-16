#pragma once

#include <memory>
#include <iostream>
#include <thread>
#include <vector>
#include <random>

#include "timer.hpp"

// Some basic semi useful client derived monitors
namespace sage::performance
{
    class cout_monitor : public performance::timer_monitor
    {
    public:
        void add_measurement(std::chrono::milliseconds duration_in_ms) override
        {
            std::cout << "Duration: " << duration_in_ms.count() << std::endl;
        }
    };

    class performance_monitor : public performance::timer_monitor
    {
    public:
        void add_measurement(std::chrono::milliseconds duration_in_ms) override
        {
            m_measurements.push_back(duration_in_ms.count());
        }

        [[nodiscard]] std::vector<double> get_measurements() const
        {
            return m_measurements;
        }

    private:
        std::vector<double> m_measurements;
    };
}