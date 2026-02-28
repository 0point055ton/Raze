#include <iostream>
#include <memory>
#include <string>

#include "bvh.h"
#include "hittable_list.h"
#include "quad.h"
#include "triangle.h"
#include "sphere.h"
#include "material.h"
#include "renderer.h"
// #include "mesh.h"

#include "WavefrontOBJ.h"

using namespace raze;

int main()
{
    std::cout << "Hello Raze Renderer!" << std::endl;

    HittableList world;

    Vector3f purple_rgb = vectorFromColor(97, 70, 138);
    Vector3f bluish_rgb = vectorFromColor(72, 40, 168);
    std::shared_ptr<Material> red = std::make_shared<Lambertian>(purple_rgb);
    std::shared_ptr<Material> metal = std::make_shared<Metal>(Vector3f(0.7f, 0.6f, 0.5f), 0.1f);
    std::shared_ptr<Material> fuzzy_metal = std::make_shared<Metal>(bluish_rgb, 0.0f);

    obj::Load obj(true);

    std::string filename = "cat/cat.obj";

    if (!obj.load(filename))
    {
        std::cout << "Error: Could not load " << filename << "\n";
    }
    else
    {
        auto face = obj.face.vertex.v.begin();

        for (const auto& size : obj.face.vertex.s)
        {
            int p0 = *(face + 0);
            int p1 = *(face + 1);
            int p2 = *(face + 2);

            Vector3f a(obj.vertex.v[p0 * 3], obj.vertex.v[p0 * 3 + 1], obj.vertex.v[p0 * 3 + 2]);
            Vector3f b(obj.vertex.v[p1 * 3], obj.vertex.v[p1 * 3 + 1], obj.vertex.v[p1 * 3 + 2]);
            Vector3f c(obj.vertex.v[p2 * 3], obj.vertex.v[p2 * 3 + 1], obj.vertex.v[p2 * 3 + 2]);

            world.add(std::make_shared<Triangle>(a, b, c, red));

            face += size;
        }
    }

    world.add(std::make_shared<Sphere>(Vector3f(-4.5f, 1.f - 1.075f, -4.5f), 1.f, fuzzy_metal));
    world.add(std::make_shared<Sphere>(Vector3f(4.5f, 1.f - 1.075f, -4.5f), 1.f, fuzzy_metal));
    
    world.add(std::make_shared<Quad>(Vector3f(-50.f,-1.075f, 50.f), Vector3f(100.f, 0.f, 0.f), Vector3f(0.f, 0.f,-100.f), metal));

    world = HittableList(std::make_shared<BVH>(world));

    Config config = loadConfigFromJSON("config.json");
    Renderer renderer(config);
    renderer.render(world, config.multithreading);
    renderer.saveToFile();

    return 0;
}