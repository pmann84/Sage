#pragma once

#include <ostream>
#include <sstream>

namespace sage::term::cursor
{
    inline std::ostream &up(std::ostream &stream)
    {
        return stream << "\033[1A";
    }

    inline std::ostream &down(std::ostream &stream)
    {
        return stream << "\033[1B";
    }

    inline std::ostream &left(std::ostream &stream)
    {
        return stream << "\033[1D";
    }

    inline std::ostream &right(std::ostream &stream)
    {
        return stream << "\033[1C";
    }

    struct up_by
    {
        size_t amount;
    };

    inline std::ostream &operator<<(std::ostream &stream, const up_by &up)
    {
        stream << "\033[" << up.amount << "A";
        return stream;
    }

    struct down_by
    {
        size_t amount;
    };

    inline std::ostream &operator<<(std::ostream &stream, const down_by &down)
    {
        stream << "\033[" << down.amount << "B";
        return stream;
    }

    struct left_by
    {
        size_t amount;
    };

    inline std::ostream &operator<<(std::ostream &stream, const left_by &left)
    {
        stream << "\033[" << left.amount << "D";
        return stream;
    }

    struct right_by
    {
        size_t amount;
    };

    inline std::ostream &operator<<(std::ostream &stream, const right_by &right)
    {
        stream << "\033[" << right.amount << "C";
        return stream;
    }
}