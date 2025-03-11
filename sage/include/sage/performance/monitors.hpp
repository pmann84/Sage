#pragma once

#include <memory>
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <sstream>

#include "timer.hpp"

// Some basic semi useful client derived monitors
namespace sage::performance
{
    class cout_monitor final : public timer_monitor
    {
    public:
        void add_measurement(std::chrono::milliseconds duration_in_ms) override
        {
            std::cout << "Duration: " << duration_in_ms.count() << std::endl;
        }
    };

    class performance_monitor final : public timer_monitor
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

        [[nodiscard]] double total() const {
            return std::accumulate(m_measurements.begin(), m_measurements.end(), 0.0);
        }

        [[nodiscard]] std::string s_total() const {
            auto ms = std::chrono::duration<double>(total() * 0.001);
            auto secs = std::chrono::duration_cast<std::chrono::seconds>(ms);
            ms -= std::chrono::duration_cast<std::chrono::milliseconds>(secs);
            auto mins = std::chrono::duration_cast<std::chrono::minutes>(secs);
            secs -= std::chrono::duration_cast<std::chrono::seconds>(mins);
            const auto hour = std::chrono::duration_cast<std::chrono::hours>(mins);
            mins -= std::chrono::duration_cast<std::chrono::minutes>(hour);

            std::stringstream ss;
            ss << hour.count() << " Hours : " << mins.count() << " Minutes : " << secs.count() << " Seconds : " << ms.count() << " Milliseconds";
            return ss.str();
        }

    private:
        std::vector<double> m_measurements;
    };
}