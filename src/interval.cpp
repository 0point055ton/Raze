#include "interval.h"

template<typename T>
const raze::Interval<T> raze::Interval<T>::empty = raze::Interval<T>(INF, -INF);

template<typename T>
const raze::Interval<T> raze::Interval<T>::universe = raze::Interval<T>(-INF, INF);