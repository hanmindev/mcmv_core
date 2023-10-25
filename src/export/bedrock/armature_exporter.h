#ifndef MCMV_CORE_SRC_OUTPUT_BEDROCK_ARMATURE_EXPORTER_H_
#define MCMV_CORE_SRC_OUTPUT_BEDROCK_ARMATURE_EXPORTER_H_

using namespace std;

struct ExporterConfig {
  float fps;
  bool exportRotation = true;
  bool exportPosition = true;
};

class IArmatureModelExporter {
 public:
  virtual void export_armature_model(string path, string name) = 0;
};

class IArmatureAnimationExporter {
 public:
  virtual void export_armature_animation(string path, string name, ExporterConfig config) = 0;
};

#endif //MCMV_CORE_SRC_OUTPUT_BEDROCK_ARMATURE_EXPORTER_H_
