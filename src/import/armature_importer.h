#ifndef MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
#define MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_

#include "../armature/joint.h"

struct JointMotion {
  Quaternion rotation;
  Vector3 offset;
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
};

#endif //MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
