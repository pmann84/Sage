#include <sage/channel/channel.hpp>

#include <string>
#include <thread>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

int main() {
    auto [pr, cr] = sage::channel::make_channel<std::string>();

    std::thread producer_thread{[](sage::channel::channel<std::string> pr) {
        std::this_thread::sleep_for(100ms);
        for (int i = 0; i < 100; ++i) {
            pr.send(std::to_string(i));
        }
    }, std::move(pr)};

    while(true) {
        auto result = cr.recv();
        if (!result) break;
        std::cout << *result << std::endl;
    }

    producer_thread.join();
    return 0;
}