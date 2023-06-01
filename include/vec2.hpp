#ifndef VEC2_HPP
#define VEC2_HPP

#include <tuple>

template <typename T>
struct Vec2
{
    T x;
    T y;

    Vec2(T x, T y) {
        this->x = x;
        this->y = y;
    }

    bool operator<(const Vec2<T> &rhs) const {
        if (x == rhs.x) {
            return y < rhs.y;
        }
        return x < rhs.x;
    }

    bool operator==(const Vec2<T> &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

#endif
