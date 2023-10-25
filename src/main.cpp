#include "import/bvh/bvh_importer.h"
#include "import/bb/bb_importer.h"
#include "armature/armature_format_adapter.h"
#include "export/bedrock/bedrock_exporter.h"



int main() {


//############# Animation Importer ##############################

  BvhImporter importer("../data/test.bvh");

  Model bvhModel = importer.get_model();
  Animation bvhAnim = importer.get_animation();

//############# Model Importer ##################################

  BBImporter bb_importer("../data/playermodel.bbmodel");

  Model BBModel = bb_importer.get_model();
  
//###############################################################



  ProcessorConfig config = {
      bvhModel,
      BBModel,
      build_joint_map(bvhModel, BBModel, {
          {"root", "Hip"},
          {"body", "Spine"},
          {"head", "Head"},

          {"elbow_r", "Right_Elbow"},
          {"arm_r", "Right_Arm"},
          {"shoulder_r", "Right_Shoulder"},

          {"knee_r", "Right_Knee"},
          {"leg_r", "Right_Thigh"},

          {"elbow_l", "Left_Elbow"},
          {"arm_l", "Left_Arm"},
          {"shoulder_l", "Left_Shoulder"},

          {"knee_l", "Left_Knee"},
          {"leg_l", "Left_Thigh"}
      })
  };

  auto FormattedModel = ArmatureFormatAdapter(config);

  for (auto& frame : bvhAnim.frames) {
      FormattedModel.push_motion_frame(frame);
  }

  auto formattedAnim = FormattedModel.get_animation();

  auto be = BedrockExporter(BBModel, formattedAnim);

  ExporterConfig ec = {
      bvhAnim.fps,
  };

  be.export_armature_animation("../data", "test", ec);
  return 0;

  
}