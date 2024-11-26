#include <string>
#include <vector>
#include "ast.h"

// Expression class for number literals.
NumberExpressionAST::NumberExpressionAST(double Value)
  : Value(Value)
{};

// Expression class for referencing a variable.
VariableExpressionAST::VariableExpressionAST(const std::string &Name) 
  : Name(Name) 
{};

// Expression class for a binary operator.
BinaryExpressionAST::BinaryExpressionAST(char Operator, std::unique_ptr<ExpressionAST> LHS, 
                      std::unique_ptr<ExpressionAST> RHS) 
  : Operator(Operator)
  , LHS(std::move(LHS))
  , RHS(std::move(RHS)) 
{};

// Expression class for function calls. 
CallExpressionAST::CallExpressionAST(const std::string &Callee, std::vector<std::unique_ptr<ExpressionAST>> Arguments) 
  : Callee(Callee)
  , Arguments(std::move(Arguments))
{};

// Class which represents the prototype of a function: its name, and its argument names. 
PrototypeAST::PrototypeAST(const std::string &Name, std::vector<std::string> Arguments)
  : Name(Name)
  , Arguments(std::move(Arguments))
{};

// Class which represents the function itself.
FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Prototype, std::unique_ptr<ExpressionAST> Body)
  : Prototype(std::move(Prototype))
  , Body(std::move(Body))
{};

