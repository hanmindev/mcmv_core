#include "quaternion.h"

#ifndef MCMV_CORE_VECTOR3_H
#define MCMV_CORE_VECTOR3_H

class ZeroScaleException : public std::exception {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "Cannot normalize a zero vector";
    }
};

class Vector3 {
public:
    float x, y, z;

    Vector3() = default;

    Vector3(float x, float y, float z);

    Vector3 operator+(const Vector3 &v) const;

    Vector3 operator-(const Vector3 &v) const;

    Vector3 operator*(float f) const;

    Vector3 &operator=(const Vector3 &v) = default;

    Vector3 &operator+=(const Vector3 &v);

    Vector3 &operator-=(const Vector3 &v);

    Vector3 &operator*=(float f);

    Vector3 operator-() const;

    [[nodiscard]] float norm() const;

    void normalize();

    void scale_to(float f);

    void rotate(const Quaternion &q);
};

#endif //MCMV_CORE_VECTOR3_H