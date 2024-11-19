#include <string>
#include "lexer.h"
#include "ast.h"

// Base class for all expressions.
class ExpressionAST {
public:
  virtual ~ExpressionAST() = default;
};

// Expression class for number literals.
class NumberExpressionAST : public ExpressionAST {
  double Value;

public:
  NumberExpressionAST(double Value) : Value(Value) {}
};

// Expression class for referencing a variable.
class VariableExpressionAST : public ExpressionAST {
  std::string Name;

public:
  VariableExpressionAST(const std::string &Name) : Name(Name) {}
};

// Expression class for a binary operator.
class BinaryExpressionAST : public ExpressionAST {
  char Operator; 
  std::unique_ptr<ExpressionAST> LHS, RHS;

public:
  BinaryExpressionAST(char Operator, std::unique_ptr<ExpressionAST> LHS, 
                      std::unique_ptr<ExpressionAST> RHS) : Operator(Operator), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

// Expression class for function calls. 
class CallExpressionAST : public ExpressionAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExpressionAST>> Arguments;

public:
  CallExpressionAST(const std::string &Callee, std::vector<std::unique_ptr<ExpressionAST>> Arguments) : Callee(Callee), Arguments(std::move(Arguments)) {}
};

// Class which represents the prototype of a function: its name, and its argument names. 
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Arguments;

public:
  PrototypeAST(const std::string &Name, std::vector<std::string> Arguments) :
    Name(Name), Arguments(std::move(Arguments)) {}
};

// Class which represents the function itself.
class FunctionAST {
  std::unique_ptr<PrototypeAST> Prototype;
  std::unique_ptr<ExpressionAST> Body;
public:
  FunctionAST(std::unique_ptr<PrototypeAST> Prototype, std::unique_ptr<ExpressionAST> Body) : Prototype(std::move(Prototype)), Body(std::move(Body)) {}
};

static int CurrentToken;
static int GetNextToken() {
  return CurrentToken = GetToken();
}
