#include "joint.h"

Joint::Joint(string name, int parent_index, int index) {
    this->name = std::move(name);
    this->parent_index = parent_index;
    this->index = index;
}