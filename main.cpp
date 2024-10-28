enum class TokenType : char {
  FuncIdentifier,
  FuncKeyword,
  VoidKeyword,
};

const std::unordered_map<std::string_view, TokenType> keywords = {
    {"function", TokenType::FuncKeyword}, {"void", TokenType::VoidKeyword}};

