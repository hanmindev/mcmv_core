#include "bb_importer.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <utility>
using json = nlohmann::json;

BBImporter::BBImporter(const string &file_name) {
    std::ifstream f(file_name);
    json data = json::parse(f);

    bb_parser = make_unique<BBParser>(data);
}

BBImporter::BBImporter(const string &file_name, string root_name) {
  std::ifstream f(file_name);
  json data = json::parse(f);

  bb_parser = make_unique<BBParser>(data, std::move(root_name));
}

Model BBImporter::get_model() {
  return Model {
      bb_parser->get_model()
  };
}

Animation BBImporter::get_animation() {
    return Animation {
        {},
        0
    };
}
