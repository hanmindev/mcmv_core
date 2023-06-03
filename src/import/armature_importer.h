#ifndef MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
#define MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_

#include "../armature/joint.h"

struct JointMotion {
  Quaternion rotation;
  Vector3 offset;
};

template<typename T>
class ConcurrentItem {
 public:
  ConcurrentItem(T item) : item(item) {}
  T item;
  mutex mutex = std::mutex();
};

class ArmatureModel {
 public:
  int joint_count;
  Joint *joints;

  ArmatureModel(int joint_count, Joint *joints) : joint_count(joint_count), joints(joints) {}
  ~ArmatureModel() = default;
};

class IArmatureImporter {
 public:
  virtual ~IArmatureImporter() = default;

  virtual ArmatureModel *get_model() = 0;
  virtual vector<JointMotion *> get_animation_frames() = 0;
  virtual ConcurrentItem<vector<JointMotion *>> *get_concurrent_animation_frames() = 0;
};

#endif //MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
