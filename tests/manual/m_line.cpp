#include <gtest/gtest.h>

#include "import/bvh/bvh_importer.h"
#include "import/bb/bb_importer.h"
#include "armature/armature_format_adapter.h"
#include "export/bedrock/bedrock_exporter.h"
#include "export/bedrock/armature_exporter.h"

const string DATA_PATH = "../tests/manual/line/";

TEST(Manual, Curved_Line) {
  BvhImporter bvh_importer(DATA_PATH + "curved_line.bvh");

  auto bvh_model = bvh_importer.get_model();
  auto bvh_animation = bvh_importer.get_animation_frames();

  BBImporter bb_importer(DATA_PATH + "curved_line.bbmodel");

  auto bb_model = bb_importer.get_model();

  ProcessorConfig config = {
      bvh_model,
      bb_model,
      build_joint_map(bvh_model, bb_model, {
          {"a", "a"},
          {"b", "b"},
          {"c", "c"},
          {"d", "d"},
          {"e", "e"},
      }),
      true,
      false
  };

  auto af = ArmatureFormatAdapter(config);

  for (auto &frame : bvh_animation) {
    af.push_motion_frame(frame);
  }

  auto be = BedrockExporter(bb_model, af.get_output_motion_frames());

  ExporterConfig ec = {
      60,
  };

  be.export_armature_animation(DATA_PATH, "curved_line", ec);
}

TEST(Manual, Forked_Line) {
  BvhImporter bvh_importer(DATA_PATH + "forked_line.bvh");

  auto bvh_model = bvh_importer.get_model();
  auto bvh_animation = bvh_importer.get_animation_frames();

  BBImporter bb_importer(DATA_PATH + "forked_line.bbmodel");

  auto bb_model = bb_importer.get_model();

  ProcessorConfig config = {
      bvh_model,
      bb_model,
      build_joint_map(bvh_model, bb_model, {
          {"a", "a"},
          {"b", "b"},
          {"c", "c"},
          {"d", "d"},
          {"e", "e"},
          {"f", "f"},
          {"g", "g"},
      }),
      true,
      false
  };

  auto af = ArmatureFormatAdapter(config);

  for (auto &frame : bvh_animation) {
    af.push_motion_frame(frame);
  }

  auto be = BedrockExporter(bb_model, af.get_output_motion_frames());

  ExporterConfig ec = {
      60,
  };

  be.export_armature_animation(DATA_PATH, "forked_line", ec);
}