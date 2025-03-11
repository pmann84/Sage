#include <sage/performance/monitors.hpp>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(PerformanceMonitorTests, TestPerformanceMonitorContainsMeasurementsAdded)
{
    sage::performance::performance_monitor perf_monitor;
    perf_monitor.add_measurement(std::chrono::milliseconds(100));
    perf_monitor.add_measurement(std::chrono::milliseconds(200));
    perf_monitor.add_measurement(std::chrono::milliseconds(300));
    perf_monitor.add_measurement(std::chrono::milliseconds(400));
    perf_monitor.add_measurement(std::chrono::milliseconds(500));

    auto measurements = perf_monitor.get_measurements();
    ASSERT_THAT(measurements, ::testing::ContainerEq(std::vector<double>({100, 200, 300, 400, 500})));
}

TEST(PerformanceMonitorTests, TestPerformanceMonitorTotalIsCorrect)
{
    sage::performance::performance_monitor perf_monitor;
    perf_monitor.add_measurement(std::chrono::milliseconds(100));
    perf_monitor.add_measurement(std::chrono::milliseconds(200));
    perf_monitor.add_measurement(std::chrono::milliseconds(300));
    perf_monitor.add_measurement(std::chrono::milliseconds(400));
    perf_monitor.add_measurement(std::chrono::milliseconds(500));

    ASSERT_THAT(perf_monitor.total(), testing::DoubleEq(1500));
}

TEST(PerformanceMonitorTests, TestPerformanceMonitorSTotalIsCorrect)
{
    sage::performance::performance_monitor perf_monitor;
    perf_monitor.add_measurement(std::chrono::milliseconds(100));
    perf_monitor.add_measurement(std::chrono::milliseconds(200));
    perf_monitor.add_measurement(std::chrono::milliseconds(300));
    perf_monitor.add_measurement(std::chrono::milliseconds(400));
    perf_monitor.add_measurement(std::chrono::milliseconds(500));

    ASSERT_THAT(perf_monitor.s_total(), testing::StrEq("00:00:01:500"));
}

TEST(PerformanceMonitorTests, TestPerformanceMonitorAverageIsCorrect)
{
    sage::performance::performance_monitor perf_monitor;
    perf_monitor.add_measurement(std::chrono::milliseconds(100));
    perf_monitor.add_measurement(std::chrono::milliseconds(200));
    perf_monitor.add_measurement(std::chrono::milliseconds(300));
    perf_monitor.add_measurement(std::chrono::milliseconds(400));
    perf_monitor.add_measurement(std::chrono::milliseconds(500));

    ASSERT_THAT(perf_monitor.average(), testing::DoubleEq(300));
}

TEST(PerformanceMonitorTests, TestPerformanceMonitorSAverageIsCorrect)
{
    sage::performance::performance_monitor perf_monitor;
    perf_monitor.add_measurement(std::chrono::milliseconds(100));
    perf_monitor.add_measurement(std::chrono::milliseconds(200));
    perf_monitor.add_measurement(std::chrono::milliseconds(300));
    perf_monitor.add_measurement(std::chrono::milliseconds(400));
    perf_monitor.add_measurement(std::chrono::milliseconds(500));

    ASSERT_THAT(perf_monitor.s_average(), testing::StrEq("00:00:00:300"));
}