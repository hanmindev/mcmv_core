
#include "bvh_importer.h"

BvhImporter::BvhImporter(const string &file_name) {
  auto file_reader = make_unique<FileReader>(file_name);
  auto bvh_lexer = make_unique<BvhLexer>(std::move(file_reader));
  this->bvh_parser = make_unique<BvhParser>(std::move(bvh_lexer));
}
Model BvhImporter::get_model() {
  return Model {
    bvh_parser->get_model()
  };
}
Animation BvhImporter::get_animation() {
  return Animation {
    bvh_parser->get_animation_frames(),
    bvh_parser->frame_rate
  };
}