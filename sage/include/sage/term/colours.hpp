#pragma once

#include <sstream>
#include <stdint.h>

#define DEFINE_ANSI_CODE(x, y) constexpr std::string_view x = y;
#define  ANSI_CODE(x) "\033[xm";

namespace sage::term
{
    constexpr std::string_view ANSI_PREFIX = "\033[";
    constexpr std::string_view ANSI_POSTFIX = "m";
    constexpr std::string_view ANSI_RGB_PREFIX = "38;2;";

    inline std::string ansi_code(const std::string_view code) {
        std::stringstream ss;
        ss << ANSI_PREFIX << code << ANSI_POSTFIX;
        return ss.str();
    }

    inline std::ostream &ansi_code(std::ostream &stream, const std::string_view code)
    {
        return stream << ansi_code(code);
    }

    inline std::ostream &f(std::ostream &stream, const std::string_view code)
    {
        return ansi_code(stream, code);
    }

    inline std::string f(const std::string_view code) {
        return ansi_code(code);
    }

    namespace fg
    {
        DEFINE_ANSI_CODE(BLACK, "30")
        DEFINE_ANSI_CODE(DARK_RED, "31")
        DEFINE_ANSI_CODE(DARK_GREEN, "32")
        DEFINE_ANSI_CODE(DARK_YELLOW, "33")
        DEFINE_ANSI_CODE(DARK_BLUE, "34")
        DEFINE_ANSI_CODE(DARK_MAGENTA, "35")
        DEFINE_ANSI_CODE(DARK_CYAN, "36")
        DEFINE_ANSI_CODE(DARK_WHITE, "37")
        DEFINE_ANSI_CODE(RED, "91")
        DEFINE_ANSI_CODE(GREEN, "92")
        DEFINE_ANSI_CODE(YELLOW, "93")
        DEFINE_ANSI_CODE(BLUE, "94")
        DEFINE_ANSI_CODE(MAGENTA, "95")
        DEFINE_ANSI_CODE(CYAN, "96")
        DEFINE_ANSI_CODE(WHITE, "97")

        // 0-255 range colour values
        struct colour
        {
            uint8_t r, g, b;
        };

        inline std::ostream &operator<<(std::ostream &stream, const colour &colour)
        {
            return stream << ANSI_PREFIX << ANSI_RGB_PREFIX << colour.r << ";" << colour.g << ";" << colour.b << ANSI_POSTFIX;
        }

        inline std::ostream &
        black(std::ostream &stream)
        {
            return ansi_code(stream, BLACK);
        }

        inline std::ostream &dark_red(std::ostream &stream)
        {
            return ansi_code(stream, DARK_RED);
        }

        inline std::ostream &dark_green(std::ostream &stream)
        {
            return ansi_code(stream, DARK_GREEN);
        }

        inline std::ostream &dark_yellow(std::ostream &stream)
        {
            return ansi_code(stream, DARK_YELLOW);
        }

        inline std::ostream &dark_blue(std::ostream &stream)
        {
            return ansi_code(stream, DARK_BLUE);
        }

        inline std::ostream &dark_magenta(std::ostream &stream)
        {
            return ansi_code(stream, DARK_MAGENTA);
        }

        inline std::ostream &dark_cyan(std::ostream &stream)
        {
            return ansi_code(stream, DARK_CYAN);
        }

        inline std::ostream &dark_white(std::ostream &stream)
        {
            return ansi_code(stream, DARK_WHITE);
        }

        inline std::ostream &red(std::ostream &stream)
        {
            return ansi_code(stream, RED);
        }

        inline std::ostream &green(std::ostream &stream)
        {
            return ansi_code(stream, GREEN);
        }

        inline std::ostream &yellow(std::ostream &stream)
        {
            return ansi_code(stream, YELLOW);
        }

        inline std::ostream &blue(std::ostream &stream)
        {
            return ansi_code(stream, BLUE);
        }

        inline std::ostream &magenta(std::ostream &stream)
        {
            return ansi_code(stream, MAGENTA);
        }

        inline std::ostream &cyan(std::ostream &stream)
        {
            return ansi_code(stream, CYAN);
        }

        inline std::ostream &white(std::ostream &stream)
        {
            return ansi_code(stream, WHITE);
        }
    }

    namespace bg
    {
        DEFINE_ANSI_CODE(BLACK, "40")
        DEFINE_ANSI_CODE(DARK_RED, "41")
        DEFINE_ANSI_CODE(DARK_GREEN, "42")
        DEFINE_ANSI_CODE(DARK_YELLOW, "43")
        DEFINE_ANSI_CODE(DARK_BLUE, "44")
        DEFINE_ANSI_CODE(DARK_MAGENTA, "45")
        DEFINE_ANSI_CODE(DARK_CYAN, "46")
        DEFINE_ANSI_CODE(DARK_WHITE, "47")
        DEFINE_ANSI_CODE(RED, "101")
        DEFINE_ANSI_CODE(GREEN, "102")
        DEFINE_ANSI_CODE(YELLOW, "103")
        DEFINE_ANSI_CODE(BLUE, "104")
        DEFINE_ANSI_CODE(MAGENTA, "105")
        DEFINE_ANSI_CODE(CYAN, "106")
        DEFINE_ANSI_CODE(WHITE, "107")

        struct colour
        {
            uint8_t r, g, b;
        };

        inline std::ostream &
        operator<<(std::ostream &stream, colour &colour)
        {
            return stream << ANSI_PREFIX << ANSI_RGB_PREFIX << colour.r << ";" << colour.g << ";" << colour.b << ANSI_POSTFIX;
        }

        inline std::ostream &black(std::ostream &stream)
        {
            return ansi_code(stream, BLACK);
        }

        inline std::ostream &dark_red(std::ostream &stream)
        {
            return ansi_code(stream, DARK_RED);
        }

        inline std::ostream &dark_green(std::ostream &stream)
        {
            return ansi_code(stream, DARK_GREEN);
        }

        inline std::ostream &dark_yellow(std::ostream &stream)
        {
            return ansi_code(stream, DARK_YELLOW);
        }

        inline std::ostream &dark_blue(std::ostream &stream)
        {
            return ansi_code(stream, DARK_BLUE);
        }

        inline std::ostream &dark_magenta(std::ostream &stream)
        {
            return ansi_code(stream, DARK_MAGENTA);
        }

        inline std::ostream &dark_cyan(std::ostream &stream)
        {
            return ansi_code(stream, DARK_CYAN);
        }

        inline std::ostream &dark_white(std::ostream &stream)
        {
            return ansi_code(stream, DARK_WHITE);
        }

        inline std::ostream &red(std::ostream &stream)
        {
            return ansi_code(stream, RED);
        }

        inline std::ostream &green(std::ostream &stream)
        {
            return ansi_code(stream, GREEN);
        }

        inline std::ostream &yellow(std::ostream &stream)
        {
            return ansi_code(stream, YELLOW);
        }

        inline std::ostream &blue(std::ostream &stream)
        {
            return ansi_code(stream, BLUE);
        }

        inline std::ostream &magenta(std::ostream &stream)
        {
            return ansi_code(stream, MAGENTA);
        }

        inline std::ostream &cyan(std::ostream &stream)
        {
            return ansi_code(stream, CYAN);
        }

        inline std::ostream &white(std::ostream &stream)
        {
            return ansi_code(stream, WHITE);
        }
    }

    DEFINE_ANSI_CODE(RESET, "0");
    DEFINE_ANSI_CODE(BOLD, "1");
    DEFINE_ANSI_CODE(FAINT, "2")
    DEFINE_ANSI_CODE(BOLD_OFF, "22");
    DEFINE_ANSI_CODE(UNDERLINE, "4");
    DEFINE_ANSI_CODE(DOUBLE_UNDERLINE, "21");
    DEFINE_ANSI_CODE(UNDERLINE_OFF, "24");
    DEFINE_ANSI_CODE(INVERSE, "7");
    DEFINE_ANSI_CODE(INVERSE_OFF, "27");
    DEFINE_ANSI_CODE(SLOW_BLINK, "5");
    DEFINE_ANSI_CODE(FAST_BLINK, "6");
    DEFINE_ANSI_CODE(BLINK_OFF, "25");
    DEFINE_ANSI_CODE(SUPERSCRIPT, "73");
    DEFINE_ANSI_CODE(SUBSCRIPT, "74");
    DEFINE_ANSI_CODE(NO_SCRIPT, "75");

    inline std::ostream &reset(std::ostream &stream)
    {
        return ansi_code(stream, RESET);
    }

    inline std::ostream &bold(std::ostream &stream)
    {
        return ansi_code(stream, BOLD);
    }

    inline std::ostream &bold_off(std::ostream &stream)
    {
        return ansi_code(stream, BOLD_OFF);
    }

    inline std::ostream &underline(std::ostream &stream)
    {
        return ansi_code(stream, UNDERLINE);
    }

    inline std::ostream &double_underline(std::ostream &stream)
    {
        return ansi_code(stream, DOUBLE_UNDERLINE);
    }

    inline std::ostream &underline_off(std::ostream &stream)
    {
        return ansi_code(stream, UNDERLINE_OFF);
    }

    inline std::ostream &inverse(std::ostream &stream)
    {
        return ansi_code(stream, INVERSE);
    }

    inline std::ostream &inverse_off(std::ostream &stream)
    {
        return ansi_code(stream, INVERSE_OFF);
    }

    inline std::ostream &slow_blink(std::ostream &stream)
    {
        return ansi_code(stream, SLOW_BLINK);
    }

    inline std::ostream &fast_blink(std::ostream &stream)
    {
        return ansi_code(stream, FAST_BLINK);
    }

    inline std::ostream &blink_off(std::ostream &stream)
    {
        return ansi_code(stream, BLINK_OFF);
    }
}

// See a full list of ANSI codes here - https://en.wikipedia.org/wiki/ANSI_escape_code