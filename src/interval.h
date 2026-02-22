#ifndef INTERVAL_H
#define INTERVAL_H

#include "constants.h"

namespace raze
{
    template<typename T>
    class Interval {
    public:
        Interval() : min(INF), max(-INF) {}
        Interval(T min, T max) : min(min), max(max) {}

        Interval(const Interval<T>& a, const Interval<T>& b) 
        {
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }

        T size() const { return max - min; }
        bool contains(T value) const { return value >= min && value <= max; }
        bool surrounds(T value) const { return value > min && value < max; }

        T clamp(T value) const 
        {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }

        Interval<T> expand(T delta) const
        {
            T padding = delta / 2.f;
            return Interval<T>(min - padding, max + padding);
        }

    public:
        T min, max;
        static const Interval empty, universe;
    };
}

#endif // INTERVAL_H