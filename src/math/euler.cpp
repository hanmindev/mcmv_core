#define _USE_MATH_DEFINES

#include "euler.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

Euler::Euler() {
    this->order = Order::xyz;
    this->x = this->y = this->z = 0;
}

Euler::Euler(Order order) {
    this->order = order;
    this->x = this->y = this->z = 0;

}

Euler::Euler(Order order, float x, float y, float z) {
    this->order = order;
    this->x = x;
    this->y = y;
    this->z = z;
}

Euler::Euler(Order order, Quaternion q) {
    float x2 = q.x + q.x;
    float y2 = q.y + q.y;
    float z2 = q.z + q.z;
    float xx = q.x * x2;
    float xy = q.x * y2;
    float xz = q.x * z2;
    float yy = q.y * y2;
    float yz = q.y * z2;
    float zz = q.z * z2;
    float wx = q.w * x2;
    float wy = q.w * y2;
    float wz = q.w * z2;

    float m11 = (1 - (yy + zz));
    float m21 = (xy + wz);
    float m31 = (xz - wy);

    float m12 = (xy - wz);
    float m22 = (1 - (xx + zz));
    float m32 = (yz + wx);

    float m13 = (xz + wy);
    float m23 = (yz - wx);
    float m33 = (1 - (xx + yy));

    switch (order) {
        case Order::xyz:
            this->y = asin(std::min(std::max(m13, -1.0f), 1.0f));

            if (abs(m13) < 0.9999999) {
                this->x = atan2(-m23, m33);
                this->z = atan2(-m12, m11);
            } else {
                this->x = atan2(m32, m22);
                this->z = 0;
            }
            break;

        case Order::yxz:
            this->x = asin(-std::min(std::max(m23, -1.0f), 1.0f));

            if (abs(m23) < 0.9999999) {
                this->y = atan2(m13, m33);
                this->z = atan2(m21, m22);
            } else {
                this->y = atan2(-m31, m11);
                this->z = 0;
            }
            break;

        case Order::zxy:
            this->x = asin(std::min(std::max(m32, -1.0f), 1.0f));

            if (abs(m32) < 0.9999999) {
                this->y = atan2(-m31, m33);
                this->z = atan2(-m12, m22);
            } else {
                this->y = 0;
                this->z = atan2(m21, m11);
            }
            break;

        case Order::zyx:
            this->y = asin(-std::min(std::max(m31, -1.0f), 1.0f));

            if (abs(m31) < 0.9999999) {
                this->x = atan2(m32, m33);
                this->z = atan2(m21, m11);
            } else {
                this->x = 0;
                this->z = atan2(-m12, m22);
            }
            break;

        case Order::yzx:
            this->z = asin(std::min(std::max(m21, -1.0f), 1.0f));

            if (abs(m21) < 0.9999999) {
                this->x = atan2(-m23, m22);
                this->y = atan2(-m31, m11);
            } else {
                this->x = 0;
                this->y = atan2(m13, m33);
            }
            break;

        case Order::xzy:
            this->z = asin(-std::min(std::max(m12, -1.0f), 1.0f));

            if (abs(m12) < 0.9999999) {
                this->x = atan2(m32, m22);
                this->y = atan2(m13, m11);
            } else {
                this->x = atan2(-m23, m33);
                this->y = 0;
            }
            break;

    }

    this->x *= 180 / M_PI;
    this->y *= 180 / M_PI;
    this->z *= 180 / M_PI;

}

Quaternion Euler::to_quaternion() const {
    float qx, qy, qz, qw;

    float rx = x * M_PI / 180;
    float ry = y * M_PI / 180;
    float rz = z * M_PI / 180;

    float c1 = cosf(rx / 2);
    float c2 = cosf(ry / 2);
    float c3 = cosf(rz / 2);

    float s1 = sinf(rx / 2);
    float s2 = sinf(ry / 2);
    float s3 = sinf(rz / 2);

    switch (order) {
        case Order::xyz:
            qx = s1 * c2 * c3 + c1 * s2 * s3;
            qy = c1 * s2 * c3 - s1 * c2 * s3;
            qz = c1 * c2 * s3 + s1 * s2 * c3;
            qw = c1 * c2 * c3 - s1 * s2 * s3;
            break;
        case Order::yxz:
            qx = s1 * c2 * c3 + c1 * s2 * s3;
            qy = c1 * s2 * c3 - s1 * c2 * s3;
            qz = c1 * c2 * s3 - s1 * s2 * c3;
            qw = c1 * c2 * c3 + s1 * s2 * s3;
            break;
        case Order::zxy:
            qx = s1 * c2 * c3 - c1 * s2 * s3;
            qy = c1 * s2 * c3 + s1 * c2 * s3;
            qz = c1 * c2 * s3 + s1 * s2 * c3;
            qw = c1 * c2 * c3 - s1 * s2 * s3;
            break;
        case Order::zyx:
            qx = s1 * c2 * c3 - c1 * s2 * s3;
            qy = c1 * s2 * c3 + s1 * c2 * s3;
            qz = c1 * c2 * s3 - s1 * s2 * c3;
            qw = c1 * c2 * c3 + s1 * s2 * s3;
            break;
        case Order::yzx:
            qx = s1 * c2 * c3 + c1 * s2 * s3;
            qy = c1 * s2 * c3 + s1 * c2 * s3;
            qz = c1 * c2 * s3 - s1 * s2 * c3;
            qw = c1 * c2 * c3 - s1 * s2 * s3;
            break;
        case Order::xzy:
            qx = s1 * c2 * c3 - c1 * s2 * s3;
            qy = c1 * s2 * c3 - s1 * c2 * s3;
            qz = c1 * c2 * s3 + s1 * s2 * c3;
            qw = c1 * c2 * c3 + s1 * s2 * s3;
            break;

    }

    return {qx, qy, qz, qw};
}
