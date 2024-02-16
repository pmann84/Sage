# sage::performance

## Overview
This is a simple RAII timer for C++, which provides a hook to gather timed measurements.

While this implementation works fine, it is worth noting that you should always check what you are measuring. As this implementation relys on destruction of the timer object when it goes out of scope, depending on the order of initialisation of other objects in the scope it may time the destruction of these other objects in the same scope. In C++ class destructors are called in reverse order to which they were initialised i.e. the last constructed object gets destructed first, so where you place your timer might be important.

Short article about the motivation for and evolution of this code: https://constreference.wordpress.com/2019/08/09/measuring-performance-of-c-code/

## Example Usage
Here is an example of how one might use this library of monitors

```c++
////////////////////////////////////////////////////////////////////////////////////
// Random utility function
////////////////////////////////////////////////////////////////////////////////////
int random_int_in_range(int start, int end)
{
    const std::uniform_int_distribution<int> uniform_dist(start, end);
    std::random_device rd;
    std::default_random_engine rand(rd());
    return uniform_dist(rand);
}

////////////////////////////////////////////////////////////////////////////////////
// Entry point
////////////////////////////////////////////////////////////////////////////////////
int main()
{
    // Simple monitor that just spits out measurement to cout
    std::cout << "Cout Monitor: " << std::endl;
    cout_monitor coutmonitor;
    for (int i = 0; i < 5; ++i)
    {
        performance::timer timer(coutmonitor);
        std::this_thread::sleep_for(std::chrono::milliseconds(random_int_in_range(0, 1000)));
    }

    // Timer with a monitor that can store the individual results
    std::cout << "Performance Monitor: " << std::endl;
    performance_monitor perf_monitor;
    for (int i = 0; i < 5; ++i)
    {
        performance::timer timer(perf_monitor);
        std::this_thread::sleep_for(std::chrono::milliseconds(random_int_in_range(0, 1000)));
    }

    for (auto& m: perf_monitor.get_measurements())
    {
        std::cout << m << ", ";
    }
    std::cout << std::endl;

    // Convenience wrapper timer
    std::cout << "Performance Monitor Convenience Function: " << std::endl;
    performance_monitor perf_conv_monitor;
    for (int i = 0; i < 5; ++i)
    {
        performance::measure(
            perf_conv_monitor,
            []()
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(random_int_in_range(0, 1000)));
            }
        );
    }

    for (auto& m: perf_conv_monitor.get_measurements())
    {
        std::cout << m << ", ";
    }
    std::cout << std::endl;

return 0;
}

```