#ifndef MCMV_CORE_PARSER_H
#define MCMV_CORE_PARSER_H

#include "bvh_lexer.h"
#include "../armature_importer.h"

enum class ChannelOrder {
  Xposition,
  Yposition,
  Zposition,
  Xrotation,
  Yrotation,
  Zrotation

};

class BvhParser {
 public:
  explicit BvhParser(unique_ptr<BvhLexer> lexer);

  vector<Joint> get_model();

  vector<JointMotion *> get_animation_frames();

 private:
  BvhToken current_token = tok_eof;
  BvhToken next_token = tok_start;

  string current_string;
  string next_string;

  unique_ptr<BvhLexer> lexer;

  vector<vector<ChannelOrder>> order;


  int frame_count;

  bool parse();

  void update_token();
  vector<Joint> model;
  vector<JointMotion *> animation_frames;

  void parse_model();
  void handle_root();
  void handle_joint(int parent_index);
  void parse_motion();
  void parse_frame();
  string handle_spaced_string();
  void handle_offset(Vector3 &offset);
  void handle_channels();
  void handle_end_site();
};

#endif //MCMV_CORE_PARSER_H