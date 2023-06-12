#ifndef MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
#define MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_

#include "../armature/joint.h"
#include <mutex>

struct JointMotion {
  Quaternion rotation;
  Vector3 offset;
};

struct Model {
  vector<Joint> joints;
};

struct Animation {
  vector<JointMotion *> frames;
  float fps;
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
  virtual Model get_model() = 0;
  virtual Animation get_animation() = 0;
//  virtual ConcurrentItem<vector<JointMotion *>> *get_concurrent_animation_frames() = 0;
};

#endif //MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
