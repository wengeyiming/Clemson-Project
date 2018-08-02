#include <iostream>
#include <typeinfo>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include "ast.h"

const Literal* IdentNode::eval() const { 
  const Literal* val = TableManager::getInstance().getLiteral(ident);
  return val;
}

const Literal* CallNode::eval() const{
  TableManager& tm = TableManager::getInstance();
  if(!tm.checkFunction(ident)){
    std::exception up = std::exception();
    throw up;
  }
  tm.pushScope();
  tm.getSuite(ident)->eval();
  tm.popScope();
  return nullptr;
}

const Literal* FunctionNode::eval() const {
  TableManager::getInstance().insert(ident,suite);
  return nullptr;
}

void SuiteNode::insert(Node* i)
{
  stmts.push_back(i);
}

const Literal* SuiteNode::eval() const {
  if (stmts.empty()) return nullptr;
  else
    {
      for(const Node* n : stmts){
      if (n) 
        {
          n->eval();
          if(TableManager::getInstance().checkName("__RETURN__"))
            return TableManager::getInstance().getLiteral("__RETURN__");
        }
      else
        throw std::string("A suite node is nullptr");
      }
      return nullptr;
    }
}


const Literal* IfNode::eval() const{
  if(!test) return nullptr;
  const Literal* lit = test->eval();
  if(!lit) throw std::string("test boolean operator is unvaild");

  if(lit->isTrue())
  {
    ifSuite->eval();
  }
  else if(elseSuite)
  {
    elseSuite->eval();
  }
  return nullptr;
}


const Literal* ReturnNode::eval() const {
  if(!node){
    const Literal* res=new IntLiteral(0);
    PoolOfNodes::getInstance().add(res);
    TableManager::getInstance().insert(name, res);
    return 0;
  }
  const Literal* res=node->eval();
  res->print();
  TableManager::getInstance().insert(name, res);
  return res;
}


const Literal* PrintNode::eval() const
{
  if (!node) {
    return nullptr;
  }
  node->eval()->print();
  return nullptr;
}


//BinaryNode
AsgBinaryNode::AsgBinaryNode(Node* left, Node* right) : 
  BinaryNode(left, right) { 
}

const Literal* AsgBinaryNode::eval() const { 
  if (!left || !right) {
    throw "error";
  }
  const Literal* res = right->eval();
  const std::string n = static_cast<IdentNode*>(left)->getIdent();
  TableManager::getInstance().insert(n,res);
  return res;
}

const Literal* AddBinaryNode::eval() const { 
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).operator+(*y);
}

const Literal* SubBinaryNode::eval() const { 
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return ((*x)-(*y));
}

const Literal* MulBinaryNode::eval() const { 
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return ((*x)*(*y));
}

const Literal* DivBinaryNode::eval() const { 
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return ((*x)/(*y));
}



const Literal* PctBinaryNode::eval() const { 
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return ((*x)%(*y));
}

const Literal* DbStarBinaryNode::eval() const { 
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return ((*x)^(*y));
}

const Literal* DbSlashBinaryNode::eval() const { 
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).DbSlash(*y);
}

const Literal* LessBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).Less(*y);
}

const Literal* GreaterBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).Greater(*y);
}

const Literal* EqualEqualBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).EqualEqual(*y);
}

const Literal* GreaterEqualBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).GreaterEqual(*y);
}

const Literal* LessEqualBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).LessEqual(*y);
}

const Literal* NotEqualBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).NotEqual(*y);
}
