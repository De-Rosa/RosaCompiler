#ifndef PARSER_H
#define PARSER_H 

extern int CurrentToken;
std::unique_ptr<FunctionAST> ParseTopLevelExpression();
std::unique_ptr<FunctionAST> ParseDefinition();
std::unique_ptr<PrototypeAST> ParseExtern();

void SetDefaultPrecedence();
int GetNextToken();

#endif
