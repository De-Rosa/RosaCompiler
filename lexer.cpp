#include <string>
#include "lexer.h"

std::string IdentifierString;
double NumberValue;

int GetToken() {
  // Get first non-whitespace character.
  static int LastChar = ' ';
  while (isspace(LastChar))
    LastChar = getchar();

  // If the first character is a letter get an identifier token.
  if (isalpha(LastChar)) {
    IdentifierString = LastChar;
    while (isalnum(LastChar = getchar()))
      IdentifierString += LastChar;

    // Check if the identifier string is equal to the function identifier. 
    if (IdentifierString == "function") return Token::Function;
    if (IdentifierString == "extern") return Token::Extern;
    // Otherwise, return a generic identifier. 
    return Token::Identifier;
  }

  // If the first character is a number get the number token.
  if (isdigit(LastChar) || LastChar == '.') {
    std::string NumberString;
    do {
      NumberString += LastChar; 
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumberValue = strtod(NumberString.c_str(), 0);
    return Token::Number;
  }

  // If the first character is a hashtag comment the rest of the line out. 
  if (LastChar == '#')
  {
    do LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
    if (LastChar != EOF) return GetToken();
  }

  // Check if it is the end of the file. 
  if (LastChar == EOF) return Token::EndOfFile;

  // If is not any of the above, return the character as its ASCII value. 
  int Char = LastChar;
  LastChar = getchar();
  return Char;
}
