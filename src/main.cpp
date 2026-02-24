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

    std::shared_ptr<Material> red = std::make_shared<Lambertian>(Vector3f(1.0f, 0.2f, 0.2f));

    // Vector3f a(-4.f, -4.f, 0.f);
    // Vector3f b(4.f, -4.f, 0.f);
    // Vector3f c(4.f, -4.f, 0.f);
    // Vector3f d(4.f, -4.f, 0.f);
    // Vector3f e(0.f, 0.f, 0.f);

    // world.add(std::make_shared<Triangle>(a, b, c, red));

    obj::Load obj(true);

    std::string filename = "cat/cat.obj";

    if (!obj.load(filename))
    {
        std::cout << "Error: Could not load " << filename << "\n";
    }
    else
    {
        // std::vector<Vector3f> vertices;
        // obj::copy(obj.vertex, vertices);
        
        // std::vector<Vector3f> normals;
        // obj::copy(obj.normal, normals);

        // std::vector<std::vector<int>> faceIndexLists;
        // obj::copy(obj.face.vertex, faceIndexLists);

        // Each faceIndexLists[i] is one triangle (because we enabled triangulation)
        // for (auto &face : faceIndexLists) 
        // {
        //     if (face.size() == 3) 
        //     {
        //         std::cout << "{" << face[0] << "}, {" << face[1] << "}, {" << face[2] << "}\n";
        //         std::cout << "{" << vertices[face[0]] << "}, {" << vertices[face[1]] << "}, {" << vertices[face[2]] << "}\n";
        //         // world.add(std::make_shared<Triangle>(vertices[face[0]], vertices[face[1]], vertices[face[2]], red));
        //     }
        // }

        auto face = obj.face.vertex.v.begin();

        for (const auto& size : obj.face.vertex.s)
        {
            int p0 = *(face + 0);
            int p1 = *(face + 1);
            int p2 = *(face + 2);

            Vector3f a(obj.vertex.v[p0 * 3], obj.vertex.v[p0 * 3 + 1], obj.vertex.v[p0 * 3 + 2]);
            Vector3f b(obj.vertex.v[p1 * 3], obj.vertex.v[p1 * 3 + 1], obj.vertex.v[p1 * 3 + 2]);
            Vector3f c(obj.vertex.v[p2 * 3], obj.vertex.v[p2 * 3 + 1], obj.vertex.v[p2 * 3 + 2]);

            // std::cout << "{" << a << "}, {" << b << "}, {" << c << "}\n";
            world.add(std::make_shared<Triangle>(a, b, c, red));

            face += size;
        }

        // world.add(std::make_shared<Triangle>(Vector3f(-1.f, -1.f, 0.f), Vector3f(1.f, -1.f, 0.f), Vector3f(0.f, 1.5f, 0.f), red));

        // std::cout << "vertex count: " << vertices.size() << "\n";
        // std::cout << "normal count: " << normals.size() << "\n";
    }

    world = HittableList(std::make_shared<BVH>(world));

    // Mesh m("cat.obj");

    Config config;
    config.aspect_ratio      = 1.f;
    config.image_width       = 720;
    config.samples_per_pixel = 50;
    config.max_depth         = 50;

    config.vertival_fov      = 40.f;
    config.look_from         = Vector3f(0.f , 4.f, 10.f);
    config.look_at           = Vector3f(0.f , 1.5f, 0.f);

    config.defocus_angle     = 0.f;
    config.focus_distance    = 10.f;
    config.shade_with_normals = false;

    raze::Renderer renderer(config);
    renderer.render(world, true);
    renderer.saveToJPEG("stb_image_render", 3);

    return 0;
}