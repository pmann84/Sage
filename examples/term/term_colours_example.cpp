#include <iostream>
#include <print>

#include "sage/term/colours.hpp"

using namespace sage::term;

int main() {
    std::cout << fg::green << underline << bg::dark_blue << "Hello, World!" << reset << std::endl;
    std::cout << fg::red << bold << bg::dark_green << "Hello, another World!" << reset << std::endl;

    std::cout << std::format("{}{}This{} is a {}formatted {}hello{}",f(fg::DARK_RED), f(SLOW_BLINK), f(BLINK_OFF), f(BOLD), f(BOLD_OFF), f(RESET)) << std::endl;

    std::println("{}{}{}Merry Christmas!{}{}", f(bg::GREEN), f(fg::RED), f(FAST_BLINK), f(BLINK_OFF), f(RESET));
    return 0;
}
