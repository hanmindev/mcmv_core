#ifndef MCMV_CORE_SRC_OUTPUT_BEDROCK_BEDROCK_EXPORTER_H_
#define MCMV_CORE_SRC_OUTPUT_BEDROCK_BEDROCK_EXPORTER_H_

#include <string>
#include "armature_exporter.h"
#include "../../import/armature_importer.h"
using namespace std;

class BedrockExporter : public IArmatureAnimationExporter {
 public:
  BedrockExporter(vector<Joint> joints, vector<JointMotion *> motion_frames);

  void export_armature_animation(string path, string name, ExporterConfig config) override;

  vector<Joint> joints;
  vector<JointMotion *> motion_frames;
};

#endif //MCMV_CORE_SRC_OUTPUT_BEDROCK_BEDROCK_EXPORTER_H_
