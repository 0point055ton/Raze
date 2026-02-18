#include <iostream>
#include <memory>

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

using namespace raze;

int main()
{
    std::cout << "Hello Raze Renderer!" << std::endl;

    HittableList world;

    auto ground_material = make_shared<Lambertian>(Vector3f(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Vector3f(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = Random::getFloat();
            Vector3f center(a + 0.9 * Random::getFloat(), 0.2, b + 0.9 * Random::getFloat());

            if ((center - Vector3f(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Random::getVector3f() * Random::getVector3f();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Random::getVector3f(0.5, 1);
                    auto fuzz = Random::getFloat(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Vector3f(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Vector3f(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Vector3f(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Vector3f(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Vector3f(4, 1, 0), 1.0, material3));

    Config config;
    config.aspect_ratio      = 16.0 / 9.0;
    config.image_width       = 1280;
    config.samples_per_pixel = 1;
    config.max_depth         = 20;

    config.vertival_fov      = 20;
    config.look_from         = Vector3f(13,2,3);
    config.look_at           = Vector3f(0,0,0);

    config.defocus_angle     = 0.6;
    config.focus_distance    = 10.0;

    // shared_ptr<raze::Material> material_ground = make_shared<raze::Lambertian>(raze::Vector3f(0.5f, 0.5f, 0.5f));
    // shared_ptr<raze::Material> material_center = make_shared<raze::Lambertian>(raze::Vector3f(0.1f, 0.2f, 0.5f));
    // shared_ptr<raze::Material> material_left   = make_shared<raze::Metal>(raze::Vector3f(0.8f, 0.8f, 0.8f), 0.3f);
    // shared_ptr<raze::Material> material_right  = make_shared<raze::Metal>(raze::Vector3f(0.8f, 0.6f, 0.2f), 1.0f);
    // shared_ptr<raze::Material> material_glass  = make_shared<raze::Dielectric>(1.5f);
    // shared_ptr<raze::Material> material_bubble = make_shared<raze::Dielectric>(1.f / 1.33f);

    // raze::HittableList world;

    // world.add(make_shared<raze::Sphere>(raze::Vector3f( 0.f, -100.5f, -1.0f), 100.0f, material_ground));
    // world.add(make_shared<raze::Sphere>(raze::Vector3f( 0.f,    0.0f, -1.2f),   0.5f, material_center));
    // world.add(make_shared<raze::Sphere>(raze::Vector3f(-1.f,    0.0f, -1.0f),   0.5f, material_glass));
    // world.add(make_shared<raze::Sphere>(raze::Vector3f(-1.f,    0.0f, -1.0f),   0.4f, material_bubble));
    // world.add(make_shared<raze::Sphere>(raze::Vector3f( 1.f,    0.0f, -1.0f),   0.5f, material_right));

    // raze::Config config;
    // config.image_width = 400;
    // config.samples_per_pixel = 50;
    // config.max_depth = 20;
    // config.aspect_ratio = 16.f / 9.f;
    // config.vertival_fov = 30.f;
    // config.look_from = Vector3f(0.f, 2.f, 3.f);
    // config.look_at = Vector3f(0.f, 0.f, -1.f);
    // config.defocus_angle = 10.f;
    // config.focus_distance = (config.look_from - config.look_at).length();

    raze::Camera camera(config);
    camera.render(world);
    camera.saveToPPM("final");
    
    return 0;
}