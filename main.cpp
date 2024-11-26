#include <iostream>
#include "ast.h"
#include "parser.h"
#include "lexer.h"

static void HandleDefinition() {
  if (ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition. \n");
  } else {
    GetNextToken();
  }
}

static void HandleExtern() {
  if (ParseExtern()) {
    fprintf(stderr, "Parsed an extern. \n");
  } else {
    GetNextToken();
  }
}

static void HandleTopLevelExpression() {
  if (ParseTopLevelExpression()) {
    fprintf(stderr, "Parsed a top-level expression. \n");
  } else {
    GetNextToken();
  }
}

static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready! \n");
    switch (CurrentToken) {
      case Token::EndOfFile:
        return;
      case ';':
        GetNextToken();
        break;
      case Token::Function:
        HandleDefinition();
        break;
      case Token::Extern:
        HandleExtern();
        break;
      default:
        HandleTopLevelExpression();
        break;
    }
  }
}


int main (int argc, char *argv[]) {
  SetDefaultPrecedence();
  GetNextToken();
  MainLoop();

  return 0;
}
