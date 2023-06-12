#ifndef MCMV_CORE_SRC_IMPORT_BVH_BVH_IMPORTER_H_
#define MCMV_CORE_SRC_IMPORT_BVH_BVH_IMPORTER_H_

#include "../armature_importer.h"
#include "bvh_parser.h"

class BvhImporter : IArmatureImporter {
 public:
  explicit BvhImporter(const string &file_name);
  ~BvhImporter();

  Model get_model() override;
  Animation get_animation() override;

 private:
  FileReader *bvh_file_reader;
  BvhLexer *bvh_lexer;
  BvhParser *bvh_parser;
};

#endif //MCMV_CORE_SRC_IMPORT_BVH_BVH_IMPORTER_H_
