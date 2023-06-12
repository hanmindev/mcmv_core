#ifndef MCMV_CORE_SRC_ARMATURE_ARMATURE_FORMAT_ADAPTER_H_
#define MCMV_CORE_SRC_ARMATURE_ARMATURE_FORMAT_ADAPTER_H_

#include <unordered_map>
#include "joint.h"
#include "../thread_pool/thread_pool.h"
#include "../import/armature_importer.h"

struct ProcessorConfig {
  // input and output joints must be topologically sorted
  Model input_model;
  Model output_model;

  unordered_map<int, int> input_joint_map;

  bool output_as_local;
  bool free_frame_after_use;
};

// map of output joint index to input joint index
unordered_map<int, int> build_joint_map(Model &input_model, Model &output_model, unordered_map<string, string> string_joint_map);

class ArmatureFormatAdapter {
 public:
  explicit ArmatureFormatAdapter(ProcessorConfig &config);
  ~ArmatureFormatAdapter();
  void push_motion_frame(JointMotion motion_frame[]);

  Animation get_animation();

 private:
  Model input_model;
  Model output_model;

  unordered_map<int, int> input_joint_map;

  bool output_as_local;
  bool free_frame_after_use;

  static void globalize_motion_frame(JointMotion *motion_frame, Model model);
  void copy_motion_frame(JointMotion *motion_frame, JointMotion *output_motion_frame);
  static void localize_motion_frame(JointMotion *motion_frame, Model model);

 protected:
  void process_motion_frame(int index);
  Animation animation;
  Animation output_animation;
  int output_joint_count;
};

#endif //MCMV_CORE_SRC_ARMATURE_ARMATURE_FORMAT_ADAPTER_H_
