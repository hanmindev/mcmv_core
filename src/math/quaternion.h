#ifndef MCMV_CORE_QUATERNION_H
#define MCMV_CORE_QUATERNION_H

class Quaternion {
public:
    float x, y, z, w;

    Quaternion();

    Quaternion(float x, float y, float z, float w);

  Quaternion conjugated() const;

    Quaternion operator*(const Quaternion &q) const;

    Quaternion &operator*=(const Quaternion &q);

    Quaternion &operator=(const Quaternion &q) = default;
};

#endif //MCMV_CORE_QUATERNION_H