#include "random.h"

namespace raze
{
    std::mt19937 Random::_Generator = std::mt19937(std::random_device{}());
    std::uniform_real_distribution<float> Random::_Distribution = std::uniform_real_distribution<float>(0.f, 1.f);
}