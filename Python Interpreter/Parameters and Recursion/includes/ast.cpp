#include <iostream>
#include <typeinfo>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include "ast.h"

const Literal* IdentNode::eval() const { 
  const Literal* val = TableManager::getInstance().getEntry(ident);
  return val;
}

const Literal* CallNode::eval() const{
  TableManager& tm = TableManager::getInstance();
  if(!tm.checkFunc(ident)){
    std::cout << "Function " << ident << "Not Found" << std::endl;
    std::exception up = std::exception();
    throw up;
  }

  tm.pushScope(ident);
  if(tm.getParameter(ident)) 
    dynamic_cast<const ParameterNode*> (tm.getParameter(ident))->argeval(arguments);
  const Literal* res = tm.getSuite(ident)->eval();
  tm.popScope();
  return res;
}

const Literal* FuncNode::eval() const {
  TableManager::getInstance().insert(ident,suite);
  TableManager::getInstance().insertargs(ident,parameter);
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
          return TableManager::getInstance().getEntry("__RETURN__");
      }
      else
        throw std::string("A suite node is nullptr");
      }
      return nullptr;
    }
}

void ParameterNode::argeval(Node* node) const {
  std::vector<Node*> arg = dynamic_cast<const ArgumentNode*> (node)->getArgument();
  std::vector<Node*>::const_iterator  it = arg.begin();
  std::vector<Node*>::const_iterator pit = parameters.begin();

  while(pit != parameters.end())
  {
    if(it != arg.end())
    {
      const std::string left=(*pit)->getIdent();
      const Literal* res = (*it)->eval();
      TableManager::getInstance().insert(left,res);
      it++;
    }
    else
    {
      (*pit)->eval();
    }
    pit++;
  }
}

 const Literal* ParameterNode::eval() const {
  for(Node* n : parameters)
  {
    const std::string str = static_cast<IdentNode*>(n)->getIdent();
  }
  return 0;
 }

const Literal* ArgumentNode::eval() const
{
  std::vector<Node*>::const_iterator it = arguments.begin();
  while(it != arguments.end())
  {
    (*it)->eval();
    it++;
  }
  return 0;

}

void ArgumentNode::InsertVector(Node* node)
{
  std::vector<Node*> v = dynamic_cast<ArgumentNode*>(node)->getArgument();
  arguments.insert(arguments.end(),v.begin(),v.end());
}

const Literal* ReturnNode::eval() const {
  if(!node){
    const Literal* res=new IntLiteral(0);
    PoolOfNodes::getInstance().add(res);
    TableManager::getInstance().insert(name, res);
    return 0;
  }
  const Literal* res=node->eval();
  TableManager::getInstance().insert(name, res);
  return res;
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


const Literal* PrintNode::eval() const
{
  if (!node) {
  std::cout<<std::endl;
  return nullptr;
  }
  node->eval()->print();
  return nullptr;
}

AsgBinaryNode::AsgBinaryNode(Node* left, Node* right) : 
  BinaryNode(left, right) { 
}

const std::string AsgBinaryNode::getIdent() const
{
  const std::string name = static_cast<IdentNode*>(left)->getIdent();
  return name;
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

const Literal* GrBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).Gr(*y);
}

const Literal* EqeqBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).EqualEqual(*y);
}

const Literal* GreqBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).GrEqual(*y);
}

const Literal* LesseqBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).LessEqual(*y);
}

const Literal* NoteqBinaryNode::eval() const {
  if (!left || !right) {
    throw "error";
  }
  const Literal* x = left->eval();
  const Literal* y = right->eval();
  return (*x).NotEqual(*y);
}
