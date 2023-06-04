
#include "bvh_importer.h"

BvhImporter::BvhImporter(const string &file_name) {
    this->bvh_file_reader = new FileReader(file_name);
    this->bvh_lexer = new BvhLexer(this->bvh_file_reader);
    this->bvh_parser = new BvhParser(this->bvh_lexer);
}
vector<Joint> *BvhImporter::get_model() {
  return bvh_parser->get_model();
}
vector<JointMotion *> *BvhImporter::get_animation_frames() {
  return bvh_parser->get_animation_frames();
}
BvhImporter::~BvhImporter() {
  delete this->bvh_file_reader;
  delete this->bvh_lexer;
  delete this->bvh_parser;
}
