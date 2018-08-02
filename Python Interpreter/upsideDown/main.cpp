#include <iostream>
int yylex();

int main() {
  yylex();
  std::cout << "Translation complete" << std::endl;
  return 0;
}
