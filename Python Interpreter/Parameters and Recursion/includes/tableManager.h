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
  const Literal* getEntry(const std::string & name); //get from symbol table
  const Node* getSuite(const std::string & name);  //get from func table
  const Node* getParameter(const std::string& name);
  void  insert(const std::string& name, const Literal* node);// insert into symbol table
  void  insert(const std::string& name, const Node* node);// insert into function table
  void  insertargs(const std::string& name, const Node* node);
  bool  checkName(const std::string& name) const; // check from symnbol table
  bool  checkFunc(const std::string& name) const; // check from function table
  bool  checkCurrentName(const std::string& name) const;
  int   checkFuncScope(const std::string& name) const;
  void  pushScope(const std::string& name);
  void  popScope();

private:
  int currentScope;
  std::vector<SymbolTable> tables;
  std::vector<FunctionTable> functions;
  std::vector<FunctionTable> parameters;
  TableManager() :currentScope(0), tables(),functions(),parameters() {
    tables.push_back(SymbolTable());
    functions.push_back(FunctionTable());
    parameters.push_back(FunctionTable());
  }
};

#endif