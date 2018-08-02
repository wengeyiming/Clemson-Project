#pragma once

//  Declarations for a calculator that builds an AST
//  and a graphical representation of the AST.
//  by Brian Malloy

#include <iostream>
#include <string>
#include <map>
#include "literal.h"
#include "tableManager.h"

extern void yyerror(const char*);
extern void yyerror(const char*, const char);

class IdentNode : public Node {
public:
   IdentNode(const std::string id) : Node(), ident(id) { } 
   virtual ~IdentNode() {}
   const std::string getIdent() const { return ident; }
   virtual const Literal* eval() const;
private:
   std::string ident;
};



class IfNode : public Node{
public:
   IfNode(const Node* n, const Node* ifstmt, const Node* elsestmt):
     test(n),ifSuite(ifstmt),elseSuite(elsestmt) {
   }
   IfNode(const IfNode&) = delete;
   const IfNode* operator=(const IfNode& i) = delete;
   ~IfNode() {}
   virtual const Literal* eval() const;
protected:
   const Node* test;
   const Node* ifSuite;
   const Node* elseSuite;
};



class ReturnNode : public Node
{
public:
   ReturnNode(Node* n, std::string re="__RETURN__"):Node(),node(n),name(re){
   }
   ReturnNode(const ReturnNode&) = delete;
   const ReturnNode* operator=(const ReturnNode& r) = delete;
   ~ReturnNode() {}
   virtual const Literal* eval() const;
private:
   Node *node;
   std::string name;
  
};



class CallNode : public Node {
public:
   CallNode(const std::string id) :Node(), ident(id) {}
   virtual const Literal* eval() const; 
   virtual ~CallNode() {}
   const std::string getName() const { return ident; }
protected:
   std::string ident;

};

class FunctionNode : public Node {
public:
   FunctionNode(const std::string id, Node* stmts): Node(),ident(id),suite(stmts){
   }
   FunctionNode(const FunctionNode&) = delete;
   FunctionNode* operator=(const FunctionNode&) = delete;
   virtual ~FunctionNode() {}
   const std::string getIdent() const {return ident;}
   virtual const Literal* eval() const;
private:
   std::string ident;
   Node* suite;
};

class SuiteNode : public Node {
public:
   SuiteNode() : Node() , stmts() {}
   void insert(Node* i);
   virtual ~SuiteNode() {}
   virtual const Literal* eval() const;
private:
   std::vector<Node*> stmts;
};


class PrintNode : public Node {
public:
   PrintNode(Node* n) :Node(), node(n) {}
   virtual const Literal* eval() const;
   PrintNode* operator=(const PrintNode&) = delete;
   ~PrintNode() {}
   PrintNode(const PrintNode&) = delete;
   Node* getnode() const { return node; }
protected:
   Node *node;
};

class BinaryNode : public Node {
public:
   BinaryNode(Node* l, Node* r) : Node(), left(l), right(r) {}
   virtual const Literal* eval() const = 0;
   Node* getLeft()  const { return left; }
   Node* getRight() const { return right; }
   BinaryNode(const BinaryNode&) = delete;
   BinaryNode& operator=(const BinaryNode&) = delete;
protected:
   Node *left;
   Node *right;
};


class AsgBinaryNode : public BinaryNode {
public:
   AsgBinaryNode(Node* left, Node* right);
   virtual const Literal* eval() const;
};

class AddBinaryNode : public BinaryNode {
public:
   AddBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
   virtual const Literal* eval() const;
};

class SubBinaryNode : public BinaryNode {
public:
   SubBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
   virtual const Literal* eval() const;
};

class MulBinaryNode : public BinaryNode {
public:
   MulBinaryNode(Node* left, Node* right) : BinaryNode(left, right) {}
   virtual const Literal* eval() const;
};

class DivBinaryNode : public BinaryNode {
public:
   DivBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
   virtual const Literal* eval() const;
};

class PctBinaryNode : public BinaryNode {
public:
   PctBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
   virtual const Literal* eval() const;
};


class DbStarBinaryNode : public BinaryNode {
public:
   DbStarBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
   virtual const Literal* eval() const;
};


class DbSlashBinaryNode : public BinaryNode {
public:
   DbSlashBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
   virtual const Literal* eval() const;
};


class LessBinaryNode : public BinaryNode {
public:
   LessBinaryNode(Node* left, Node* right) : BinaryNode(left,right) {}
   virtual const Literal* eval() const;
};


class GreaterBinaryNode : public BinaryNode {
public:
   GreaterBinaryNode(Node* left, Node* right) : BinaryNode(left,right) {
     std::cout << "Great Binary Node" << std::endl;
   }
   virtual const Literal* eval() const;
};

class EqualEqualBinaryNode : public BinaryNode {
public:
   EqualEqualBinaryNode(Node* left, Node* right) : BinaryNode(left,right) {}
   virtual const Literal* eval() const;
};

class GreaterEqualBinaryNode : public BinaryNode {
public:
   GreaterEqualBinaryNode(Node* left, Node* right) : BinaryNode(left,right) {}
   virtual const Literal* eval() const;
};

class LessEqualBinaryNode : public BinaryNode {
public:
   LessEqualBinaryNode(Node* left, Node* right) : BinaryNode(left,right) {}
   virtual const Literal* eval() const;
};

class NotEqualBinaryNode : public BinaryNode {
public:
   NotEqualBinaryNode(Node* left, Node* right) : BinaryNode(left,right) {}
   virtual const Literal* eval() const;
};

class UnaryNode : public Node {
public:
   UnaryNode(Node* n) :Node(), node(n) {}
   virtual const Literal* eval() const 
   {
      if (!node) {
         throw "error";
      }
      const Literal* x = node->eval();
      return x->MiusOp();
   }
   Node* getnode() const { return node; }
   UnaryNode(const UnaryNode&) = delete;
   UnaryNode& operator=(const UnaryNode&) = delete;
protected:
   Node *node;

};