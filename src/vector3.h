#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

namespace raze
{
    template<typename T>
    class Vector3 {
    public:
        Vector3() : x(T()), y(T()), z(T()) {}
        Vector3(T value) : x(value), y(value), z(value) {}
        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

        Vector3 operator-() const { return Vector3(-x, -y, -z); }

        Vector3& operator+=(const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector3& operator/=(T scalar)
        {
            return *this *= 1 / scalar;
        }

        T length() const
        {
            return std::sqrt(lengthSquared());
        }

        T lengthSquared() const
        {
            return x * x + y * y + z * z;
        }

        T at(int axis) const
        {
            switch (axis)
            {
            case 0: return x; break;
            case 1: return y; break;
            case 2: return z; break;
            default: return x; break;
            }
        }

        bool nearZero() const
        {
            T s = 1e-8;
            return (std::fabsf(x) < s) && (std::fabsf(y) < s) && (std::fabsf(z) < s);
        }

        template<typename C>
        friend std::ostream& operator<<(std::ostream& out, const Vector3<C>& v);

    public:
        T x, y, z;
    };

    using Vector3f = Vector3<float>;

    template<typename T>
    inline std::ostream& operator<<(std::ostream& out, const Vector3<T>& v)
    {
        return out << v.x << ' ' << v.y << ' ' << v.z;
    }

    template<typename T>
    inline Vector3<T> operator+(const Vector3<T>& u, const Vector3<T>& v)
    {
        return Vector3(u.x + v.x, u.y + v.y, u.z + v.z);
    }

    template<typename T>
    inline Vector3<T> operator-(const Vector3<T>& u, const Vector3<T>& v)
    {
        return Vector3(u.x - v.x, u.y - v.y, u.z - v.z);
    }

    template<typename T>
    inline Vector3<T> operator*(const Vector3<T>& u, const Vector3<T>& v)
    {
        return Vector3(u.x * v.x, u.y * v.y, u.z * v.z);
    }

    template<typename T>
    inline Vector3<T> operator*(T scalar, const Vector3<T>& v)
    {
        return Vector3(scalar * v.x, scalar * v.y, scalar * v.z);
    }

    template<typename T>
    inline Vector3<T> operator*(const Vector3<T>& v, T scalar)
    {
        return scalar * v;
    }

    template<typename T>
    inline Vector3<T> operator/(const Vector3<T>& v, T scalar)
    {
        return (1 / scalar) * v;
    }

    template<typename T>
    inline T dot(const Vector3<T>& u, const Vector3<T>& v)
    {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    template<typename T>
    inline Vector3<T> cross(const Vector3<T>& u, const Vector3<T>& v)
    {
        return Vector3<T>(u.y * v.z - u.z * v.y, 
                          u.z * v.x - u.x * v.z, 
                          u.x * v.y - u.y * v.x);
    }

    template<typename T>
    inline Vector3<T> unitVector(const Vector3<T>& v)
    {
        return v / v.length();
    }

    template<typename T>
    inline Vector3<T> reflect(const Vector3<T>& v, const Vector3<T>& n)
    {
        return v - 2 * dot(v, n) * n;
    }

    template<typename T>
    inline Vector3<T> refract(const Vector3<T>& uv, const Vector3<T>& n, float etai_over_etat)
    {
        float cos_theta = std::fminf(dot(-uv, n), 1.f);
        Vector3f r_out_perp = etai_over_etat * (uv + cos_theta * n);
        Vector3f r_out_parallel = -std::sqrtf(std::fabsf(1.f - r_out_perp.lengthSquared())) * n;
        return r_out_perp + r_out_parallel;
    }
}

#endif // VECTOR3_H