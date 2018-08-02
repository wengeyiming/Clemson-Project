#ifndef SYMBOLTABLE__H
#define SYMBOLTABLE__H

#include <iostream>
#include <string>
#include <map>

class Literal;

class SymbolTable {
public:
  SymbolTable():table() {};
  bool isFound(const std::string& name) const;
  void insertLiteral(const std::string& name, const Literal* val);
  const Literal* getLiteral(const std::string& name) const;

private:
  std::map<std::string, const Literal*> table;
};

#endif
