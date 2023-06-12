#include <stack>
#include "armature_format_adapter.h"

unordered_map<int, int> build_joint_map(Model &input_model,
                                        Model &output_model,
                                        unordered_map<string, string> string_joint_map) {
  unordered_map<int, int> joint_map;
  // not ideal but it works
  for (int i = 0; i < output_model.joints.size(); i++) {
    auto &output_joint = output_model.joints[i];
    auto input_joint_i = string_joint_map.find(output_joint.name);
    if (input_joint_i == string_joint_map.end()) {
      throw std::runtime_error("Joint '" + output_joint.name + "' from output joints not found in map");
    }

    bool found = false;

    for (int j = 0; j < input_model.joints.size(); j++) {
      auto &input_joint = input_model.joints[j];
      if (input_joint.name == input_joint_i->second) {
        joint_map[i] = j;
        found = true;
        break;
      }
    }
    if (!found) {
      throw std::runtime_error("Joint '" + input_joint_i->second + "' not found in input joints");
    }
  }
  return joint_map;
}

ArmatureFormatAdapter::ArmatureFormatAdapter(ProcessorConfig &config) {
  this->input_model = std::move(config.input_model);
  this->output_model = std::move(config.output_model);
  this->input_joint_map = std::move(config.input_joint_map);

  this->output_as_local = config.output_as_local;
  this->free_frame_after_use = config.free_frame_after_use;
  this->output_joint_count = this->output_model.joints.size();
}

ArmatureFormatAdapter::~ArmatureFormatAdapter() {
  for (auto &motion_frame : this->animation.frames) {
    delete[] motion_frame;
  }
  for (auto &motion_frame : this->output_animation.frames) {
    delete[] motion_frame;
  }
}
void ArmatureFormatAdapter::push_motion_frame(JointMotion *motion_frame) {
  this->animation.frames.push_back(motion_frame);
  this->output_animation.frames.push_back(new JointMotion[this->output_joint_count]);
  process_motion_frame(this->animation.frames.size() - 1);
}

void ArmatureFormatAdapter::globalize_motion_frame(JointMotion *motion_frame, Model model) {
  // we can do it in one pass because the input model are topologically sorted
  for (int i = 1; i < model.joints.size(); i++) {
    int parent_index = model.joints[i].parent_index;
    motion_frame[i].offset =
        motion_frame[parent_index].offset + motion_frame[i].offset.rotated(motion_frame[parent_index].rotation);
    motion_frame[i].rotation = motion_frame[i].rotation * motion_frame[parent_index].rotation;
  }
}

void ArmatureFormatAdapter::special(JointMotion *motion_frame, Model model) {
  vector<Quaternion> rotations(model.joints.size());
  rotations[0] = model.joints[0].rotation;

  for (int i = 1; i < model.joints.size(); i++) {
    // do not touch this, took me 2 years to figure out
    rotations[i] = model.joints[i].rotation * rotations[model.joints[i].parent_index];
    motion_frame[i].rotation = rotations[model.joints[i].parent_index] * model.joints[i].rotation * motion_frame[i].rotation * model.joints[i].rotation.conjugated() * rotations[model.joints[i].parent_index].conjugated();
  }
}

void ArmatureFormatAdapter::localize_motion_frame(JointMotion *motion_frame, Model model) {
  // we can do it in one pass because the input model are topologically sorted
  for (int i = model.joints.size() - 1; i > 0; i--) {
    int parent_index = model.joints[i].parent_index;
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
  JointMotion *motion_frame = this->animation.frames[index];
  JointMotion *output_motion_frame = this->output_animation.frames[index];
  globalize_motion_frame(motion_frame, this->input_model);
  copy_motion_frame(motion_frame, output_motion_frame);

  if (this->output_as_local) {
    localize_motion_frame(output_motion_frame, this->output_model);
    special(output_motion_frame, this->output_model);
  }
  if (this->free_frame_after_use) {
    delete motion_frame;
  }
}

Animation ArmatureFormatAdapter::get_animation() {
  return this->output_animation;
}