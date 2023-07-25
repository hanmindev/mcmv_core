#ifndef MCMV_CORE_SRC_IMPORT_BB_BB_IMPORTER_H_
#define MCMV_CORE_SRC_IMPORT_BB_BB_IMPORTER_H_

#include "../armature_importer.h"
#include "bb_parser.h"

class BBImporter : IArmatureImporter {
 public:
  explicit BBImporter(const string &file_name);
  explicit BBImporter(const string &file_name, string root_name);

  Model get_model() override;
  Animation get_animation() override;

 private:
  unique_ptr<BBParser> bb_parser;
};

#endif //MCMV_CORE_SRC_IMPORT_BB_BB_IMPORTER_H_
