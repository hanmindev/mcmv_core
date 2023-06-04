#include <stack>
#include "armature_format_adapter.h"

ArmatureFormatAdapter::ArmatureFormatAdapter(ProcessorConfig &config) {
  this->input_joints = std::move(config.input_joints);
  this->output_joints = std::move(config.output_joints);
  this->input_joint_map = std::move(config.input_joint_map);

  this->output_as_local = config.output_as_local;
  this->free_frame_after_use = config.free_frame_after_use;
}

ArmatureFormatAdapter::~ArmatureFormatAdapter() {
  for (auto &motion_frame : this->motion_frames) {
    delete motion_frame;
  }
  for (auto &motion_frame : this->output_motion_frames) {
    delete motion_frame;
  }
}
void ArmatureFormatAdapter::push_motion_frame(JointMotion *motion_frame) {
  this->motion_frames.push_back(motion_frame);
  this->output_motion_frames.push_back(new JointMotion[this->output_joint_count]);
  process_motion_frame(this->motion_frames.size() - 1);
}

void ArmatureFormatAdapter::globalize_motion_frame(JointMotion *motion_frame, vector<Joint> joints) {
  // we can do it in one pass because the input joints are topologically sorted
  for (int i = 1; i < joints.size(); i++) {
    int parent_index = joints[i].parent_index;
    motion_frame[i].offset =
        motion_frame[parent_index].offset + motion_frame[i].offset.rotated(motion_frame[parent_index].rotation);
    motion_frame[i].rotation = motion_frame[i].rotation * motion_frame[parent_index].rotation;
  }
}

void ArmatureFormatAdapter::localize_motion_frame(JointMotion *motion_frame, vector<Joint> joints) {
  // we can do it in one pass because the input joints are topologically sorted
  for (int i = joints.size() - 1; i > 0; i--) {
    int parent_index = joints[i].parent_index;
    motion_frame[i].offset =
        (motion_frame[i].offset
            - motion_frame[parent_index].offset).rotated(motion_frame[parent_index].rotation.conjugated());
    motion_frame[i].rotation = motion_frame[i].rotation * motion_frame[parent_index].rotation.conjugated();
  }
}

void ArmatureFormatAdapter::copy_motion_frame(JointMotion *motion_frame, JointMotion *output_motion_frame) {
  for (int i = 0; i < this->output_joint_count; i++) {
    int input_index = this->input_joint_map.at(i);
    output_motion_frame[i].offset = motion_frame[input_index].offset;
    output_motion_frame[i].rotation = motion_frame[input_index].rotation;
  }
}

void ArmatureFormatAdapter::process_motion_frame(int index) {
  JointMotion *motion_frame = this->motion_frames[index];
  JointMotion *output_motion_frame = this->output_motion_frames[index];
  globalize_motion_frame(motion_frame, this->input_joints);
  copy_motion_frame(motion_frame, output_motion_frame);
  if (this->output_as_local) {
    localize_motion_frame(output_motion_frame, this->output_joints);
  }
  if (this->free_frame_after_use) {
    delete motion_frame;
  }
}

vector<JointMotion *> ArmatureFormatAdapter::get_output_motion_frames() {
  return this->output_motion_frames;
}
