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

    std::shared_ptr<Material> red = std::make_shared<Lambertian>(Vector3f(1.0f, 0.2f, 0.2f));
    std::shared_ptr<Material> blue = std::make_shared<Lambertian>(Vector3f(0.2f, 0.2f, 1.0f));
    std::shared_ptr<Material> metal = std::make_shared<Metal>(Vector3f(0.7f, 0.6f, 0.5f), 0.f);

    obj::Load obj(true);

    std::string filename = "skull/skull.obj";

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

            world.add(std::make_shared<Triangle>(a, b, c, metal));

            face += size;
        }
    }

    world.add(std::make_shared<Sphere>(Vector3f(-10.f, 0.f, 0.f), 5.f, red));
    world.add(std::make_shared<Sphere>(Vector3f(10.f, 0.f, 0.f), 5.f, blue));

    world = HittableList(std::make_shared<BVH>(world));

    Config config = loadConfigFromJSON("config.json");
    Renderer renderer(config);
    renderer.render(world, config.multithreading);
    renderer.saveToFile();

    return 0;
}