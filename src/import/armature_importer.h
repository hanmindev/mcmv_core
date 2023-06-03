#ifndef MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
#define MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_

#include "../armature/joint.h"

struct JointMotion {
  Quaternion rotation;
  Vector3 offset;
};

#endif //MCMV_CORE_SRC_IMPORT_ARMATURE_IMPORTER_H_
