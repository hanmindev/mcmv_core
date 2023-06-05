#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include "vector3.h"
#include "quaternion.h"

Vector3::Vector3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3 Vector3::operator+(const Vector3 &v) const {
    return {
            this->x + v.x,
            this->y + v.y,
            this->z + v.z
    };
}

Vector3 Vector3::operator-(const Vector3 &v) const {
    return {
            this->x - v.x,
            this->y - v.y,
            this->z - v.z
    };
}

Vector3 Vector3::operator*(float f) const {
    return {
            this->x * f,
            this->y * f,
            this->z * f
    };
}

Vector3 &Vector3::operator+=(const Vector3 &v) {
    return *this = *this + v;
}

Vector3 &Vector3::operator-=(const Vector3 &v) {
    return *this = *this - v;
}

Vector3 &Vector3::operator*=(float f) {
    return *this = *this * f;
}

Vector3 Vector3::operator-() const {
    return {
            -this->x,
            -this->y,
            -this->z
    };
}

float Vector3::norm() const {
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

void Vector3::normalize() {
    float norm = this->norm();
    if (norm == 0) {
        throw ZeroScaleException();
    }

    this->x /= norm;
    this->y /= norm;
    this->z /= norm;
}

void Vector3::scale_to(float f) {
    float norm = this->norm();
    if (norm == 0) {
        throw ZeroScaleException();
    }
    float s = f / norm;
    this->x *= s;
    this->y *= s;
    this->z *= s;
}

void Vector3::rotate(const Quaternion &q) {
  Quaternion p = {this->x, this->y, this->z, 0};
  Quaternion q_inv = q.conjugated();
  Quaternion r = q * p * q_inv;
  this->x = r.x;
  this->y = r.y;
  this->z = r.z;
}

Vector3 Vector3::rotated(const Quaternion &q) const {
  Quaternion p = {this->x, this->y, this->z, 0};
  Quaternion q_inv = q.conjugated();
  Quaternion r = q * p * q_inv;
  return {r.x, r.y, r.z};
}