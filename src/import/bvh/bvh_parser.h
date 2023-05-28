#ifndef MCMV_CORE_PARSER_H
#define MCMV_CORE_PARSER_H

#include "bvh_lexer.h"

class Parser {
 public:
  explicit Parser(Lexer *lexer);

  ~Parser();

 private:
  Token current_token = tok_eof;
  Token next_token = tok_start;
  Lexer *lexer;



};

#endif //MCMV_CORE_PARSER_H