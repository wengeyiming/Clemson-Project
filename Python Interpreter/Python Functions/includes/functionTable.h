#ifndef FUNCTIONTABLE__H
#define FUNCTIONTABLE__H

#include <iostream>
#include <string>
#include <map>

class Node;

class FunctionTable {
public:
  FunctionTable(): functions () {};
  bool isFound(const std::string& name) const;
  void insertSuite(const std::string& name, const Node* suite);
  const Node* getSuite(const std::string& name) const;

private:
  std::map<std::string, const Node*> functions;
};



#endif
