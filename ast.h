#ifndef AST_H
#define AST_H 

class ExpressionAST; 
class NumberExpressionAST;
class VariableExpressionAST;
class BinaryExpressionAST;
class CallExpressionAst;
class PrototypeAST;
class FunctionAST;

static int CurrentToken;
static int GetNextToken();

#endif
