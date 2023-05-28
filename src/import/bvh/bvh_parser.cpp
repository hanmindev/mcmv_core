#include "bvh_parser.h"

Parser::Parser(Lexer *lexer) {
    this->lexer = lexer;

}
Parser::~Parser() {
  delete this->lexer;
}
