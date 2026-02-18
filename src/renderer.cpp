#include "renderer.h"

namespace raze
{
    Renderer::Renderer(const Config& config) : _Config(config) {}

    void Renderer::render(const Hittable& world) 
    {
        (void)world;
    }

    void Renderer::renderRegion(const Hittable& world, std::vector<Color> result)
    {
        (void)world;
        (void)result;
    }
}