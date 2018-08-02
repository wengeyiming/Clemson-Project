#ifndef TABLEMANAGER__H
#define TABLEMANAGER__H

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include "symbolTable.h"
#include "functionTable.h"

class Literal;

class TableManager {
public:
  static  TableManager& getInstance();
  const Literal* getLiteral(const std::string & name);
  const Node* getSuite(const std::string & name);  
  void  insert(const std::string& name, const Literal* node);// insert into symbolTable
  void  insert(const std::string& name, const Node* node);// insert into functionTable
  bool  checkName(const std::string& name) const; 
  bool  checkFunction(const std::string& name) const; 
  void  pushScope();
  void  popScope();
private:
  int currentScope;
  std::vector<SymbolTable> tables;
  std::vector<FunctionTable> functions;
  TableManager() :currentScope(0), tables(),functions() {
    tables.push_back(SymbolTable());
    functions.push_back(FunctionTable());
  }
};


#endif