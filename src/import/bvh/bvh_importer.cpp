
#include "bvh_importer.h"

BvhImporter::BvhImporter(const string &file_name) {
    this->bvh_file_reader = new FileReader(file_name);
    this->bvh_lexer = new BvhLexer(this->bvh_file_reader);
    this->bvh_parser = new BvhParser(this->bvh_lexer);
}
ArmatureModel *BvhImporter::get_model() {
  return nullptr;
}
vector<JointMotion *> BvhImporter::get_animation_frames() {
  return vector<JointMotion *>();
}
