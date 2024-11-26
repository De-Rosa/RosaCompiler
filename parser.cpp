#include <string>
#include <vector>
#include <map>
#include "ast.h"
#include "lexer.h"
#include "parser.h"

int CurrentToken;

int GetNextToken() {
  return CurrentToken = GetToken();
}

// Log an error for an expression and return a nullptr.
std::unique_ptr<ExpressionAST> LogError(const char *String) {
  fprintf(stderr, "Error: %s\n", String);
  return nullptr;
}

// Log an error for a prototype and return a nullptr.
std::unique_ptr<PrototypeAST> LogErrorP(const char *String) {
  LogError(String);
  return nullptr;
}

static std::unique_ptr<ExpressionAST> ParseExpression();

// Parse a number expression by creating an expression with the current number value.
static std::unique_ptr<ExpressionAST> ParseNumberExpression() {
  auto Result = std::make_unique<NumberExpressionAST>(NumberValue);
  GetNextToken();
  return std::move(Result);
}

// Parse an expression inside parentheses. Returns a nullptr on error.
static std::unique_ptr<ExpressionAST> ParseParenthesesExpression() {
  GetNextToken();
  auto Inside = ParseExpression();
  if (!Inside) return nullptr;
  if (CurrentToken != ')') return LogError("Expected ')'.");
  GetNextToken();
  return Inside;
}

static std::unique_ptr<ExpressionAST> ParseIdentifierExpression() {
  std::string IdName = IdentifierString;
  GetNextToken();
  if (CurrentToken != '(') return std::make_unique<VariableExpressionAST>(IdName);

  GetNextToken();
  std::vector<std::unique_ptr<ExpressionAST> > Arguments;
  if (CurrentToken != ')') {
    while (true) {
      if (auto Argument = ParseExpression()) Arguments.push_back(std::move(Argument));
      else return nullptr;

      if (CurrentToken == ')') break;
      if (CurrentToken != ',') return LogError("Expected ')' or ','.");

      GetNextToken();
    }
  }

  GetNextToken();
  return std::make_unique<CallExpressionAST>(IdName, std::move(Arguments));
}

// Parses an arbitrary expression.
static std::unique_ptr<ExpressionAST> ParsePrimary() {
  switch (CurrentToken) {
    default:
      return LogError("Expected an expression, recieved unknown token.");
    case Token::Identifier:
      return ParseIdentifierExpression();
    case Token::Number: 
      return ParseNumberExpression();
    case '(':
      return ParseParenthesesExpression();
  }
}

static std::map<char, int> BinaryOpPrecedence;
static std::unique_ptr<ExpressionAST> ParseBinaryOpRHS(int ExpressionPrecedence, std::unique_ptr<ExpressionAST> LHS);

// Binary Operation Precidence, holds the precidence for each operator defined.
static int GetTokenPrecedence() {
  if (!isascii(CurrentToken)) return -1;

  int TokenPrecedence = BinaryOpPrecedence[CurrentToken];
  if (TokenPrecedence <= 0) return -1;
  return TokenPrecedence;
}

void SetDefaultPrecedence() {
  BinaryOpPrecedence['<'] = 10;
  BinaryOpPrecedence['+'] = 20;
  BinaryOpPrecedence['-'] = 20;
  BinaryOpPrecedence['*'] = 40;
}

static std::unique_ptr<ExpressionAST> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS) return nullptr;
  return ParseBinaryOpRHS(0, std::move(LHS));
}

// Parse a binary operation recursively, respecting the precedence.
static std::unique_ptr<ExpressionAST> ParseBinaryOpRHS(int ExpressionPrecedence, std::unique_ptr<ExpressionAST> LHS) {
  while (true) {
    int TokenPrecedence = GetTokenPrecedence();
    if (TokenPrecedence < ExpressionPrecedence) return LHS;

    int BinaryOperation = CurrentToken;
    GetNextToken();

    auto RHS = ParsePrimary();
    if (!RHS) return nullptr;

    int NextPrecedence = GetTokenPrecedence();
    if (TokenPrecedence < NextPrecedence) {
      RHS = ParseBinaryOpRHS(TokenPrecedence + 1, std::move(RHS));
      if (!RHS) return nullptr;
    }

    LHS = std::make_unique<BinaryExpressionAST>(BinaryOperation, std::move(LHS), std::move(RHS));
  }
}

static std::unique_ptr<PrototypeAST> ParsePrototype() {
  if (CurrentToken != Token::Identifier) return LogErrorP("Expected function name in prototype.");

  std::string FunctionName = IdentifierString;
  GetNextToken();

  if (CurrentToken != '(') return LogErrorP("Expectd '(' in prototype.");

  std::vector<std::string> ArgumentNames;
  while (GetNextToken() == Token::Identifier) {
    ArgumentNames.push_back(IdentifierString);
  }

  if (CurrentToken != ')') return LogErrorP("Expected ')' in prototype.");
  GetNextToken();
  return std::make_unique<PrototypeAST>(FunctionName, std::move(ArgumentNames));
}

std::unique_ptr<FunctionAST> ParseDefinition() {
  GetNextToken();
  auto Prototype = ParsePrototype();
  if (!Prototype) return nullptr;
  if (auto Expression = ParseExpression()) return std::make_unique<FunctionAST>(std::move(Prototype), std::move(Expression));
  return nullptr;
}

std::unique_ptr<PrototypeAST> ParseExtern() {
  GetNextToken();
  return ParsePrototype();
}

std::unique_ptr<FunctionAST> ParseTopLevelExpression() {
  if (auto Expression = ParseExpression()) {
    auto Prototype = std::make_unique<PrototypeAST>("", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(Prototype), std::move(Expression));
  }
  return nullptr;
}
