#include <fstream>
#include "bedrock_exporter.h"
#include "../../import/bb/bb_parser.h"
#include "../../math/euler.h"

void BedrockExporter::export_armature_animation(string path, string name, ExporterConfig config) {
  json j;

  j["format_version"] = "1.8.0";

  json bones;

  for (auto &joint : joints) {
    json bone;

    json rotation;
    json position;

    for (int i = 0; i < motion_frames.size(); i++) {
      auto &motion_frame = motion_frames[i];
      string frame_name = std::to_string(i / config.fps);
      Euler e = Euler(Order::zyx, motion_frame->rotation);
      rotation[frame_name] = {e.x, e.y, e.z};
      position[frame_name] = {motion_frame->offset.x, motion_frame->offset.y, motion_frame->offset.z};
    }

    bone["rotation"] = rotation;
    bone["position"] = position;

    bones[joint.name] = bone;
  }
  j["animations"]["animation."s + name + "new"s]["description"]["identifier"] = bones;

  std::filesystem::path pl(path);

  std::filesystem::create_directories(pl);

  pl.append(name + ".animation.json");

  ofstream o(pl);
  o << j.dump(4);
  o.close();
}

BedrockExporter::BedrockExporter(vector<Joint> joints, vector<JointMotion *> motion_frames) {
  this->joints = std::move(joints);
  this->motion_frames = std::move(motion_frames);
}
