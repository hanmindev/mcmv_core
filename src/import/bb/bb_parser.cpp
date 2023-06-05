#include "bb_parser.h"
#include "../../math/euler.h"

#include <utility>
BBParser::BBParser(json data) {
  this->model_root = std::move(data["outliner"][0]);
}

json find_outliner(json &data, string &root_name) {
  for (auto &item : data["children"]) {
    if (item["name"] == root_name) {
      return item;
    }

    auto result = find_outliner(item, root_name);
    if (result != nullptr) {
      return result;
    }
  }
  return nullptr;
}

BBParser::BBParser(json data, string root_name) {
  for (auto &item : data["outliner"]) {
    auto result = find_outliner(item, root_name);
    if (result != nullptr) {
      this->model_root = std::move(result);
      break;
    }
  }
}

vector<Joint> BBParser::get_model() {
  return model;
}

bool BBParser::parse() {
  parse_model();

  return true;
}

void BBParser::parse_model_node(json &node, int index, int parent_index, Vector3 parent_global_offset) {
  auto name = node["name"];
  auto children = node["children"];
  auto origin = node["origin"];
  auto rotation = node["rotation"];

  Vector3 global_offset = parent_global_offset;
  if (!origin.is_null()) {
    global_offset.x = origin[0].get<float>();
    global_offset.y = origin[1].get<float>();
    global_offset.z = origin[2].get<float>();
  }

  Quaternion rot = {0, 0, 0, 1};
  if (!rotation.is_null()) {
    rot = {Euler(Order::zyx, rotation[0].get<float>(), rotation[1].get<float>(), rotation[2].get<float>()).to_quaternion()};
  }

  auto joint = Joint(name, parent_index, index);
  joint.offset = global_offset - parent_global_offset;
  joint.rotation = rot;

  model.push_back(joint);

  for (auto &child : children) {
    parse_model_node(child, index + 1, index, global_offset);
  }
}

void BBParser::parse_model() {
  parse_model_node(model_root, 0, -1, Vector3());
}