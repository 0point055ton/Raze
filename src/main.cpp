#include <iostream>
#include <memory>

#include "bvh.h"
#include "hittable_list.h"
#include "quad.h"
#include "sphere.h"
#include "material.h"
#include "renderer.h"

using namespace raze;

int main()
{
    std::cout << "Hello Raze Renderer!" << std::endl;

    // HittableList world;

    // auto ground_material = make_shared<Lambertian>(Vector3f(0.5f, 0.5f, 0.5f));
    // world.add(make_shared<Sphere>(Vector3f(0.f, -1000.f, 0.f), 1000.f, ground_material));

    // for (int a = -11; a < 11; ++a) 
    // {
    //     for (int b = -11; b < 11; ++b) 
    //     {
    //         auto choose_mat = Random::getFloat();
    //         Vector3f center(a + 0.9f * Random::getFloat(), 0.2f, b + 0.9f * Random::getFloat());

    //         if ((center - Vector3f(4.f, 0.2f, 0.f)).length() > 0.9f) 
    //         {
    //             shared_ptr<Material> sphere_material;

    //             if (choose_mat < 0.8f) 
    //             {
    //                 auto albedo = Random::getVector3f() * Random::getVector3f();
    //                 sphere_material = make_shared<Lambertian>(albedo);
    //                 world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
    //             } 
    //             else if (choose_mat < 0.95f) 
    //             {
    //                 auto albedo = Random::getVector3f(0.5f, 1.f);
    //                 auto fuzz = Random::getFloat(0.f, 0.5f);
    //                 sphere_material = make_shared<Metal>(albedo, fuzz);
    //                 world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
    //             } 
    //             else 
    //             {
    //                 sphere_material = make_shared<Dielectric>(1.5f);
    //                 world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
    //             }
    //         }
    //     }
    // }

    // auto material1 = make_shared<Dielectric>(1.5f);
    // world.add(make_shared<Sphere>(Vector3f(0.f, 1.f, 0.f), 1.f, material1));

    // auto material2 = make_shared<Lambertian>(Vector3f(0.4f, 0.2f, 0.1f));
    // world.add(make_shared<Sphere>(Vector3f(-4.f, 1.f, 0.f), 1.f, material2));

    // auto material3 = make_shared<Metal>(Vector3f(0.7f, 0.6f, 0.5f), 0.f);
    // world.add(make_shared<Sphere>(Vector3f(4.f, 1.f, 0.f), 1.f, material3));

    HittableList world;

    // Materials
    auto left_red     = make_shared<Lambertian>(Vector3f(1.0f, 0.2f, 0.2f));
    auto back_green   = make_shared<Lambertian>(Vector3f(0.2f, 1.0f, 0.2f));
    auto right_blue   = make_shared<Lambertian>(Vector3f(0.2f, 0.2f, 1.0f));
    auto upper_orange = make_shared<Lambertian>(Vector3f(1.0f, 0.5f, 0.0f));
    auto lower_teal   = make_shared<Lambertian>(Vector3f(0.2f, 0.8f, 0.8f));

    // Quads
    world.add(std::make_shared<Quad>(Vector3f(-3.f,-2.f, 5.f), Vector3f(0.f, 0.f,-4.f), Vector3f(0.f, 4.f, 0.f), left_red));
    world.add(std::make_shared<Quad>(Vector3f(-2.f,-2.f, 0.f), Vector3f(4.f, 0.f, 0.f), Vector3f(0.f, 4.f, 0.f), back_green));
    world.add(std::make_shared<Quad>(Vector3f( 3.f,-2.f, 1.f), Vector3f(0.f, 0.f, 4.f), Vector3f(0.f, 4.f, 0.f), right_blue));
    world.add(std::make_shared<Quad>(Vector3f(-2.f, 3.f, 1.f), Vector3f(4.f, 0.f, 0.f), Vector3f(0.f, 0.f, 4.f), upper_orange));
    world.add(std::make_shared<Quad>(Vector3f(-2.f,-3.f, 5.f), Vector3f(4.f, 0.f, 0.f), Vector3f(0.f, 0.f,-4.f), lower_teal));

    world = HittableList(std::make_shared<BVH>(world));

    Config config;
    config.aspect_ratio      = 1.f;
    config.image_width       = 400;
    config.samples_per_pixel = 10;
    config.max_depth         = 50;

    config.vertival_fov      = 80.f;
    config.look_from         = Vector3f(0.f, 0.f, 12.f);
    config.look_at           = Vector3f(0.f , 0.f, 0.f);

    config.defocus_angle     = 0.f;
    config.focus_distance    = 10.f;

    raze::Renderer renderer(config);
    renderer.render(world, true);
    renderer.saveToPPM("quads");

    return 0;
}