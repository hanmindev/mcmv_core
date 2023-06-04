#ifndef MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
#define MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_

#include "../armature/joint.h"
#include <mutex>

struct JointMotion {
  Quaternion rotation;
  Vector3 offset;
};

template<typename T>
class ConcurrentItem {
 public:
  ConcurrentItem(T item) : item(item) {}
  T item;
  mutex c_mutex;
};

class IArmatureImporter {
 public:
  virtual vector<Joint> get_model() = 0;
  virtual vector<JointMotion *> get_animation_frames() = 0;
//  virtual ConcurrentItem<vector<JointMotion *>> *get_concurrent_animation_frames() = 0;
};

#endif //MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
