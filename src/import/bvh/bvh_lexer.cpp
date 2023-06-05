#include "bvh_lexer.h"

BvhLexer::BvhLexer(FileReader *file_reader) {
  this->file_reader = file_reader;
  this->last_char = this->file_reader->read_char();

}

void BvhLexer::read_char() {
  this->last_char = this->file_reader->read_char();
}
BvhToken BvhLexer::get_token() {
  while (isspace(this->last_char)) {
    this->read_char();
  }

  if (isalpha(this->last_char)) {
    this->current_string = "";
    while (isalnum(this->last_char) || this->last_char == '_' || this->last_char == '-') {
      this->current_string += this->last_char;
      this->read_char();
    }

    if (this->current_string == "HIERARCHY") {
      return tok_hierarchy;
    } else if (this->current_string == "ROOT") {
      return tok_root;
    } else if (this->current_string == "OFFSET") {
      return tok_offset;
    } else if (this->current_string == "CHANNELS") {
      return tok_channels;
    } else if (this->current_string == "JOINT") {
      return tok_joint;
    } else if (this->current_string == "End") {
      return tok_end;
    } else if (this->current_string == "Site") {
      return tok_site;
    } else if (this->current_string == "MOTION") {
      return tok_motion;
    } else if (this->current_string == "Frames") {
      return tok_frames;
    } else if (this->current_string == "Frame") {
      return tok_frame;
    } else if (this->current_string == "Time") {
      return tok_time;
    } else {
      return tok_string;
    }
  }

  if (isdigit(this->last_char) || this->last_char == '.' || this->last_char == '-') {
    if (this->last_char == '-') {
      this->current_string = "-";
      this->read_char();
    } else {
      this->current_string = "";
    }
    while (isdigit(this->last_char)) {
      this->current_string += this->last_char;
      this->read_char();
    }
    if (this->last_char == '.') {
      this->current_string += this->last_char;
      this->read_char();
      while (isdigit(this->last_char)) {
        this->current_string += this->last_char;
        this->read_char();
      }
    }
    return tok_number;
  }
  if (this->last_char == EOF) {
    // don't eat the EOF
    return tok_eof;
  }

  char this_char = this->last_char;
  this->read_char();
  switch (this_char) {
    case '{':return tok_l_brace;
    case '}':return tok_r_brace;
    case ':':return tok_colon;
    default:break;
  }
  return tok_error;
}

string BvhLexer::get_string() {
  return this->current_string;
}
