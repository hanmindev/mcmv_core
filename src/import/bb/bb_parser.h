#ifndef MCMV_CORE_SRC_IMPORT_BB_BB_PARSER_H_
#define MCMV_CORE_SRC_IMPORT_BB_BB_PARSER_H_

#include "../armature_importer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class BBParser {
 public:
  explicit BBParser(json bb_json);
  explicit BBParser(json bb_json, string root_name);

  vector<Joint> get_model();

 private:
  json model_root;

  vector<Joint> model;

  bool parse();
  void parse_model();

  void parse_model_node(json &node, int parent_index, Vector3 parent_global_offset);
};

#endif //MCMV_CORE_SRC_IMPORT_BB_BB_PARSER_H_
