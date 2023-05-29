#ifndef MCMV_CORE_SRC_ARMATURE_ARMATURE_FORMAT_ADAPTER_H_
#define MCMV_CORE_SRC_ARMATURE_ARMATURE_FORMAT_ADAPTER_H_

#include <unordered_map>
#include "joint.h"
struct JointMotion {
  Quaternion rotation;
  Vector3 offset;
};

struct ProcessorConfig {
  // input and output joints must be topologically sorted
  int input_joint_count;
  Joint *input_joints;
  int output_joint_count;
  Joint *output_joints;

  unordered_map<int, int> *input_joint_map;
};

class ArmatureFormatAdapter {
 public:
  explicit ArmatureFormatAdapter(ProcessorConfig *config);
  ~ArmatureFormatAdapter();
  void push_motion_frame(JointMotion motion_frame[]);

 private:
  int input_joint_count;
  Joint *input_joints;

  int output_joint_count;
  Joint *output_joints;

  unordered_map<int, int> *input_joint_map;

  vector<JointMotion *> motion_frames;
  vector<JointMotion *> output_motion_frames;

  static void globalize_motion_frame(JointMotion *motion_frame, Joint *joints, int joint_count);
  void copy_motion_frame(JointMotion *motion_frame, JointMotion *output_motion_frame);
  static void localize_motion_frame(JointMotion *motion_frame, Joint *joints, int joint_count);
  void process_motion_frame(int index);

};

#endif //MCMV_CORE_SRC_ARMATURE_ARMATURE_FORMAT_ADAPTER_H_
