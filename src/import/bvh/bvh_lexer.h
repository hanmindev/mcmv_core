#ifndef MCMV_CORE_SRC_IMPORT_BVH_BVH_LEXER_H_
#define MCMV_CORE_SRC_IMPORT_BVH_BVH_LEXER_H_

#include "../file_reader.h"

enum BvhToken {
  tok_start,
  tok_eof,
  tok_error,
  tok_hierarchy,
  tok_root,

  tok_offset,
  tok_channels,
  tok_joint,
  tok_end,
  tok_site,

  tok_l_brace,
  tok_r_brace,
  tok_colon,

  tok_motion,
  tok_frames,
  tok_frame,
  tok_time,

  tok_string,
  tok_int,
  tok_dec
};

class BvhLexer {
 public:
  explicit BvhLexer(FileReader *file_reader);
  BvhToken get_token();
  string get_string();

  ~BvhLexer();

 private:
  FileReader *file_reader;
  char last_char = ' ';
  string current_string;

  void read_char();

};

#endif //MCMV_CORE_SRC_IMPORT_BVH_BVH_LEXER_H_
