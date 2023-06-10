#include "bb_importer.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <utility>
using json = nlohmann::json;

BBImporter::BBImporter(const string &file_name) {
    std::ifstream f(file_name);
    json data = json::parse(f);

    bb_parser = new BBParser(data);
}

BBImporter::BBImporter(const string &file_name, string root_name) {
  std::ifstream f(file_name);
  json data = json::parse(f);

  bb_parser = new BBParser(data, std::move(root_name));
}

vector<Joint> BBImporter::get_model() {
  return bb_parser->get_model();
}

vector<JointMotion *> BBImporter::get_animation_frames() {
    return {};
}

BBImporter::~BBImporter() {
    delete bb_parser;
}
