#pragma once

//  Declarations for a calculator that builds an AST
//  and a graphical representation of the AST.
//  by Brian Malloy

#include <iostream>
#include <string>
#include <map>
#include "literal.h"


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

