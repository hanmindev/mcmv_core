#include "quaternion.h"

#ifndef MCMV_CORE_EULER_H
#define MCMV_CORE_EULER_H

#endif //MCMV_CORE_EULER_H

enum class Order
{   xyz,
    yxz,
    zxy,
    zyx,
    yzx,
    xzy
};

class Euler {
public:
    Order order;

    float x, y, z;

    Euler();

    explicit Euler(Order order);

    Euler(Order order, float x, float y, float z);

    Euler(Order order, Quaternion q);

    Euler &operator=(const Euler &e) = default;

    [[nodiscard]] Quaternion to_quaternion() const;
};
