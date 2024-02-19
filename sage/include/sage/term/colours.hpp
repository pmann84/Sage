#pragma once

#include <stdint.h>
#include <ostream>
#include <sstream>

namespace sage::term
{
    inline std::ostream &ansi_code(std::ostream &stream, std::string code)
    {
        return stream << "\033[" << code << "m";
    }

    namespace fg
    {
        // 0-255 range colour values
        struct colour
        {
            uint8_t r, g, b;
        };

        inline std::ostream &operator<<(std::ostream &stream, const colour &colour)
        {
            return stream << "\033[38;2;" << colour.r << ";" << colour.g << ";" << colour.b << "m";
        }

        inline std::ostream &
        black(std::ostream &stream)
        {
            return ansi_code(stream, "30");
        }

        inline std::ostream &dark_red(std::ostream &stream)
        {
            return ansi_code(stream, "31");
        }

        inline std::ostream &dark_green(std::ostream &stream)
        {
            return ansi_code(stream, "32");
        }

        inline std::ostream &dark_yellow(std::ostream &stream)
        {
            return ansi_code(stream, "33");
        }

        inline std::ostream &dark_blue(std::ostream &stream)
        {
            return ansi_code(stream, "34");
        }

        inline std::ostream &dark_magenta(std::ostream &stream)
        {
            return ansi_code(stream, "35");
        }

        inline std::ostream &dark_cyan(std::ostream &stream)
        {
            return ansi_code(stream, "36");
        }

        inline std::ostream &dark_white(std::ostream &stream)
        {
            return ansi_code(stream, "37");
        }

        inline std::ostream &red(std::ostream &stream)
        {
            return ansi_code(stream, "91");
        }

        inline std::ostream &green(std::ostream &stream)
        {
            return ansi_code(stream, "92");
        }

        inline std::ostream &yellow(std::ostream &stream)
        {
            return ansi_code(stream, "93");
        }

        inline std::ostream &blue(std::ostream &stream)
        {
            return ansi_code(stream, "94");
        }

        inline std::ostream &magenta(std::ostream &stream)
        {
            return ansi_code(stream, "95");
        }

        inline std::ostream &cyan(std::ostream &stream)
        {
            return ansi_code(stream, "96");
        }

        inline std::ostream &white(std::ostream &stream)
        {
            return ansi_code(stream, "97");
        }
    }

    namespace bg
    {
        struct colour
        {
            uint8_t r, g, b;
        };

        inline std::ostream &
        operator<<(std::ostream &stream, colour &colour)
        {
            return stream << "\033[38;2;" << colour.r << ";" << colour.g << ";" << colour.b << "m";
        }

        inline std::ostream &black(std::ostream &stream)
        {
            return ansi_code(stream, "40");
        }

        inline std::ostream &dark_red(std::ostream &stream)
        {
            return ansi_code(stream, "41");
        }

        inline std::ostream &dark_green(std::ostream &stream)
        {
            return ansi_code(stream, "42");
        }

        inline std::ostream &dark_yellow(std::ostream &stream)
        {
            return ansi_code(stream, "43");
        }

        inline std::ostream &dark_blue(std::ostream &stream)
        {
            return ansi_code(stream, "44");
        }

        inline std::ostream &dark_magenta(std::ostream &stream)
        {
            return ansi_code(stream, "45");
        }

        inline std::ostream &dark_cyan(std::ostream &stream)
        {
            return ansi_code(stream, "46");
        }

        inline std::ostream &dark_white(std::ostream &stream)
        {
            return ansi_code(stream, "47");
        }

        inline std::ostream &red(std::ostream &stream)
        {
            return ansi_code(stream, "101");
        }

        inline std::ostream &green(std::ostream &stream)
        {
            return ansi_code(stream, "102");
        }

        inline std::ostream &yellow(std::ostream &stream)
        {
            return ansi_code(stream, "103");
        }

        inline std::ostream &blue(std::ostream &stream)
        {
            return ansi_code(stream, "104");
        }

        inline std::ostream &magenta(std::ostream &stream)
        {
            return ansi_code(stream, "105");
        }

        inline std::ostream &cyan(std::ostream &stream)
        {
            return ansi_code(stream, "106");
        }

        inline std::ostream &white(std::ostream &stream)
        {
            return ansi_code(stream, "107");
        }
    }

    inline std::ostream &reset(std::ostream &stream)
    {
        return ansi_code(stream, "0");
    }

    inline std::ostream &bold(std::ostream &stream)
    {
        return ansi_code(stream, "1");
    }

    inline std::ostream &underline(std::ostream &stream)
    {
        return ansi_code(stream, "4");
    }

    inline std::ostream &inverse(std::ostream &stream)
    {
        return ansi_code(stream, "7");
    }
}

// See a full list of ANSI codes here - https://en.wikipedia.org/wiki/ANSI_escape_code
//    bold/bright off  21
//    underline off    24
//    inverse off      27