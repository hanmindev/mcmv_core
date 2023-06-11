#include <gtest/gtest.h>
#include "armature/joint.h"
#include "import/armature_importer.h"
#include "math/euler.h"
#include "armature/armature_format_adapter.h"
#include "gmock/gmock.h"
#include "../helper/helper.h"

using ::testing::FloatEq;
using testing::AllOf;
using testing::Field;

vector<Joint> get_line(int bone_count) {
  vector<Joint> model;
  model.emplace_back(to_string(0), -1, 0);

  for (int i = 1; i < bone_count; i++) {
    Joint joint(to_string(i), i - 1, i);
    joint.offset = Vector3(1, 0, 0);

    model[i - 1].children_indices.push_back(i);

    model.push_back(joint);
  }
  return model;
}

TEST(Functional, Hexagon_Inverse) {
  int joint_count = 7;
  auto input_model = get_line(joint_count);
  auto output_model = get_line(joint_count);

  ProcessorConfig config = {
      input_model,
      output_model,
      unordered_map<int, int>(
          {
              {0, 0},
              {1, 1},
              {2, 2},
              {3, 3},
              {4, 4},
              {5, 5},
              {6, 6}
          }
      ),
      true,
      false
  };

  auto af = ArmatureFormatAdapter(config);

  auto *motion_frame = new JointMotion[7];
  for (int j = 0; j < joint_count; j++) {
    motion_frame[j].offset = Vector3(0, 0, 0);
    motion_frame[j].rotation = Euler(Order::xyz, 0, 0, 60).to_quaternion();
  }
  af.push_motion_frame(motion_frame);

  auto output_frames = af.get_output_motion_frames();

  auto output_frame = output_frames[0];

  for (int j = 0; j < joint_count; j++) {
    auto output_joint = output_frame[j];

    auto &actual_vector = output_joint.offset;
    auto expected_vector = Vector3(0, 0, 1);

    EXPECT_THAT(actual_vector, Vector3Eq(expected_vector));

    auto expected_quaternion = Euler(Order::xyz, 0, 0, 60).to_quaternion();
    auto &actual_quaternion = output_joint.rotation;

    EXPECT_THAT(actual_quaternion, QuaternionEq(expected_quaternion));
  }
}
