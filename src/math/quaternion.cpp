#include "quaternion.h"

Quaternion::Quaternion() {
    x = y = z = 0;
    w = 1;
}

Quaternion::Quaternion(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Quaternion Quaternion::operator*(const Quaternion &q) const {
    return {
            q.w * this->x + q.x * this->w + q.y * this->z - q.z * this->y,
            q.w * this->y - q.x * this->z + q.y * this->w + q.z * this->x,
            q.w * this->z + q.x * this->y - q.y * this->x + q.z * this->w,
            q.w * this->w - q.x * this->x - q.y * this->y - q.z * this->z
    };
}

Quaternion &Quaternion::operator*=(const Quaternion &q) {
    float x = q.w * this->x + q.x * this->w + q.y * this->z - q.z * this->y;
    float y = q.w * this->y - q.x * this->z + q.y * this->w + q.z * this->x;
    float z = q.w * this->z + q.x * this->y - q.y * this->x + q.z * this->w;
    float w = q.w * this->w - q.x * this->x - q.y * this->y - q.z * this->z;

    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;

    return *this;
}

Quaternion Quaternion::operator-() const {
    return {
            -this->x,
            -this->y,
            -this->z,
            this->w
    };
}
