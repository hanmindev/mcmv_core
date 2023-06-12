
#include "bvh_importer.h"

BvhImporter::BvhImporter(const string &file_name) {
  this->bvh_file_reader = new FileReader(file_name);
  this->bvh_lexer = new BvhLexer(this->bvh_file_reader);
  this->bvh_parser = new BvhParser(this->bvh_lexer);
}
Model BvhImporter::get_model() {
  return Model {
      bvh_parser->get_model()
  };
}
Animation BvhImporter::get_animation() {
  return Animation {
      bvh_parser->get_animation_frames(),
      20
  };
}
BvhImporter::~BvhImporter() {
  delete this->bvh_file_reader;
  delete this->bvh_lexer;
  delete this->bvh_parser;
}
