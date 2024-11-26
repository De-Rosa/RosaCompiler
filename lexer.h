#ifndef LEXER_H
#define LEXER_H 

// https://en.wikipedia.org/wiki/Lexical_analysis
// Lexical Tokens
// Had to be defined in the header file due to incomplete type errors.
enum Token : signed int {
  Identifier = -1,
  Number = -2,
  EndOfFile = -3,
  Function = -4,
  Extern = -5,
};

extern std::string IdentifierString;
extern double NumberValue;

int GetToken();

#endif
