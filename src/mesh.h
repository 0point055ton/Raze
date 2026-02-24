// #ifndef MESH_H
// #define MESH_H

// #include <string>

// #include "hittable.h"
// #include "triangle.h"
// #include "WavefrontOBJ.h"

// namespace raze
// {
//     class Mesh : public Hittable {
//     public:
//         Mesh(const std::string& filename)
//         {
//             obj::Load file(true);

//             if (!file.load(filename))
//             {
//                 std::cout << "Error: Could not load " << filename << "\n";
//             }
//             else
//             {
//                 // std::vector<Vector3f> vertices;
//                 // obj::copy(file.vertex, vertices);
                
//                 // std::vector<Vector3f> normals;
//                 // obj::copy(file.normal, normals);

//                 // std::vector<std::vector<int>> faceIndexLists;
//                 // obj::copy(file.face.vertex, faceIndexLists);

//                 // // Each faceIndexLists[i] is one triangle (because we enabled triangulation)
//                 // for (auto &face : faceIndexLists) 
//                 // {
//                 //     if (face.size() == 3) 
//                 //     {
//                 //         _Triangles.emplace_back(face[0], face[1], face[2]);
//                 //     }
//                 // }
//             }
//         }

//         bool hit(const Ray& ray, Interval<float> ray_t, HitInfo& record) const override
//         {
//             (void)ray;
//             (void)ray_t;
//             (void)record;

//             bool hit_anything = false;

//             // for (auto& triangle : _Triangles)
//             // {
//             //     if (triangle.hit(ray, ray_t, record));
//             // }

//             return hit_anything;
//         }

//         AABB boundingBox() const override
//         {
//             return _BBox;
//         }

//     private:
//         std::vector<Triangle> _Triangles;
//         AABB _BBox;
//     };
// }

// #endif // MESH_H