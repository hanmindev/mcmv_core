#include <string>
#include <utility>
#include <vector>
#include "../math/vector3.h"

using namespace std;

#ifndef MCMV_CORE_JOINT_H
#define MCMV_CORE_JOINT_H

#endif //MCMV_CORE_JOINT_H

class Joint {
public:
    string name;
    int parent_index;
    int index;

    Vector3 offset{};
    Quaternion rotation{};

    vector<int> children_indices{};

    Joint(string name, int parent_index, int index);

    Joint &operator=(const Joint &j) = default;
};