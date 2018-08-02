#include "node.h"
#include "poolOfNodes.h"
#include <cmath>

class Literal : public Node {
public:
  virtual ~Literal() {}

  virtual const Literal* operator+(const Literal& rhs) const =0;
  virtual const Literal* opPlus(float) const =0;
  virtual const Literal* opPlus(int) const =0;

  virtual const Literal* operator*(const Literal& rhs) const =0;
  virtual const Literal* opMult(float) const =0;
  virtual const Literal* opMult(int) const =0;

  virtual const Literal* operator-(const Literal& rhs) const =0;
  virtual const Literal* opSubt(float) const =0;
  virtual const Literal* opSubt(int) const =0;

  virtual const Literal* operator/(const Literal& rhs) const =0;
  virtual const Literal* opDiv(float) const =0;
  virtual const Literal* opDiv(int) const =0;

  virtual const Literal* operator%(const Literal& rhs) const =0;
  virtual const Literal* opPct(float) const =0;
  virtual const Literal* opPct(int) const =0;


  virtual const Literal* operator^(const Literal& rhs) const =0;
  virtual const Literal* opDbStar(float) const =0;
  virtual const Literal* opDbStar(int) const =0;

  virtual const Literal* DbSlash(const Literal& rhs) const =0;
  virtual const Literal* opDbSlash(float) const =0;
  virtual const Literal* opDbSlash(int) const =0;

  virtual const Literal* Less(const Literal& rhs) const = 0;
  virtual const Literal* opLess(float) const = 0;
  virtual const Literal* opLess(int) const = 0;

  virtual const Literal* Greater(const Literal& rhs) const = 0;
  virtual const Literal* opGreater(float) const = 0;
  virtual const Literal* opGreater(int) const = 0;

  virtual const Literal* EqualEqual(const Literal& rhs) const = 0;
  virtual const Literal* opEqualEqual(float) const = 0;
  virtual const Literal* opEqualEqual(int) const = 0;

  virtual const Literal* GreaterEqual(const Literal& rhs) const = 0;
  virtual const Literal* opGreaterEqual(float) const = 0;
  virtual const Literal* opGreaterEqual(int) const = 0;

  virtual const Literal* LessEqual(const Literal& rhs) const = 0;
  virtual const Literal* opLessEqual(float) const = 0;
  virtual const Literal* opLessEqual(int) const = 0;

  virtual const Literal* NotEqual(const Literal& rhs) const = 0;
  virtual const Literal* opNotEqual(float) const = 0;
  virtual const Literal* opNotEqual(int) const = 0;
  virtual const Literal* MiusOp()  const = 0;

  virtual int isTrue() const = 0;
  virtual const Literal* eval() const = 0;
  virtual void print() const { 
    std::cout << "No Way" << std::endl; 
  }
};

class FloatLiteral: public Literal {
public:
  FloatLiteral(float _val): val(_val) {}

  virtual const Literal* operator+(const Literal& rhs) const  {
    return rhs.opPlus(val);
  }

  virtual const Literal* opPlus(float lhs) const  {
    const Literal* node = new FloatLiteral(lhs + val);
    PoolOfNodes::getInstance().add(node);
    return node; 
  }

  virtual const Literal* opPlus(int lhs) const  {
    const Literal* node = new FloatLiteral(lhs + val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator-(const Literal& rhs) const  {
    return rhs.opSubt(val);
  }

  virtual const Literal* opSubt(float lhs) const  {
    const Literal* node = new FloatLiteral(lhs - val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opSubt(int lhs) const  {
    const Literal* node = new FloatLiteral(lhs - val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator*(const Literal& rhs) const  {
    return rhs.opMult(val);
  }

  virtual const Literal* opMult(float lhs) const  {
    const Literal* node = new FloatLiteral(lhs * val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opMult(int lhs) const  {
    const Literal* node = new FloatLiteral(static_cast<float>(lhs) * val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator/(const Literal& rhs) const  {
    return rhs.opDiv(val);
  }

  virtual const Literal* opDiv(float lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(lhs / val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opDiv(int lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(lhs / val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator%(const Literal& rhs) const  {
    return rhs.opPct(val);
  }

  virtual const Literal* opPct(float lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(fmod(fmod(lhs,val)+val,val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opPct(int lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(fmod(fmod(lhs,val)+val,val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator^(const Literal& rhs) const  {
    return rhs.opDbStar(val);
  }

  virtual const Literal* opDbStar(float lhs) const  {
    const Literal* node = new FloatLiteral(pow(lhs,val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opDbStar(int lhs) const  {
    const Literal* node = new FloatLiteral(pow(static_cast<float>(lhs), val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* DbSlash(const Literal& rhs) const  {
    return rhs.opDbSlash(val);
  }

  virtual const Literal* opDbSlash(float lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(floor(lhs / val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opDbSlash(int lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(floor(lhs / val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* Less(const Literal& rhs) const {
    return rhs.opLess(val);
  }

  virtual const Literal* opLess(float lhs) const {
    const Literal* node;
    if(lhs < val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opLess(int lhs) const {
    const Literal* node;
    if(lhs < val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* Greater(const Literal& rhs) const {
    return rhs.opGreater(val);
  }

  virtual const Literal* opGreater(float lhs) const {
    const Literal* node;
    if(lhs > val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opGreater(int lhs) const {
    const Literal* node;
    if(lhs > val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);

    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* EqualEqual(const Literal& rhs) const {
    return rhs.opEqualEqual(val);
  }

  virtual const Literal* opEqualEqual(float lhs) const {
    const Literal* node;
    if(lhs == val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opEqualEqual(int lhs) const {
    const Literal* node;
    if(lhs == val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* GreaterEqual(const Literal& rhs) const {
    return rhs.opGreaterEqual(val);
  }

  virtual const Literal* opGreaterEqual(float lhs) const {
    const Literal* node;
    if(lhs >= val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opGreaterEqual(int lhs) const {
    const Literal* node;
    if(lhs >= val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* LessEqual(const Literal& rhs) const {
    return rhs.opLessEqual(val);
  }

  virtual const Literal* opLessEqual(float lhs) const {
    const Literal* node;
    if(lhs <= val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opLessEqual(int lhs) const {
    const Literal* node;
    if(lhs <= val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* NotEqual(const Literal& rhs) const {
    return rhs.opNotEqual(val);
  }

  virtual const Literal* opNotEqual(float lhs) const {
    const Literal* node;
    if(lhs != val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opNotEqual(int lhs) const {
    const Literal* node;
    if(lhs != val)
      node = new FloatLiteral(1);
    else
      node = new FloatLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* MiusOp() const  {
    const Literal* node = new FloatLiteral(-val);
    PoolOfNodes::getInstance().add(node);
    return node; 
  }

  virtual const Literal* eval() const { return this; }
  
  virtual int isTrue() const { return int(this->val);}
  
  virtual void print() const { 
    std::cout << "FLOAT: " << val << std::endl; 
  }
private:
  float val;
};

class IntLiteral: public Literal {
public:
 IntLiteral(int _val): val(_val) {}

  virtual const Literal* operator+(const Literal& rhs) const  {
    return rhs.opPlus(val);
  }

  virtual const Literal* opPlus(float lhs) const  {
    const Literal* node = new FloatLiteral(static_cast<float>(val) + lhs);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opPlus(int lhs) const  {
    const Literal* node = new IntLiteral(lhs + val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator-(const Literal& rhs) const  {
    return rhs.opSubt(val);
  }

  virtual const Literal* opSubt(float lhs) const  {
    const Literal* node = new FloatLiteral(lhs - val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opSubt(int lhs) const  {
    const Literal* node = new IntLiteral(lhs - val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator*(const Literal& rhs) const  {
    return rhs.opMult(val);
  }

  virtual const Literal* opMult(float lhs) const  {
    const Literal* node = new FloatLiteral(lhs * val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opMult(int lhs) const  {
    const Literal* node = new IntLiteral(lhs * val);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator/(const Literal& rhs) const  {
    return rhs.opDiv(val);
  }

  virtual const Literal* opDiv(float lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(lhs / val);
    PoolOfNodes::getInstance().add(node);

    return node;
  }

  virtual const Literal* opDiv(int lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new IntLiteral(floor(static_cast<float>(lhs)/ val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator%(const Literal& rhs) const  {
    return rhs.opPct(val);
  }

  virtual const Literal* opPct(float lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(fmod(fmod(lhs,val)+val,val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opPct(int lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new IntLiteral(fmod(fmod(lhs,val)+val,val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* operator^(const Literal& rhs) const  {
    return rhs.opDbStar(val);
  }

  virtual const Literal* opDbStar(float lhs) const  {
    const Literal* node = new FloatLiteral(pow(static_cast<float>(lhs), val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opDbStar(int lhs) const  {
    const Literal* node = new IntLiteral(pow(lhs, val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* DbSlash(const Literal& rhs) const  {
    return rhs.opDbSlash(val);
  }

  virtual const Literal* opDbSlash(float lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new FloatLiteral(floor(lhs / val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opDbSlash(int lhs) const  {
    if ( val == 0 ) throw std::string("Zero Division Error");
    const Literal* node = new IntLiteral(floor(static_cast<float>(lhs)/ val));
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* Less(const Literal& rhs) const {
    return rhs.opLess(val);
  }

  virtual const Literal* opLess(float lhs) const {
    const Literal* node;
    if(lhs < val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opLess(int lhs) const {
    const Literal* node;
    if(lhs < val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* Greater(const Literal& rhs) const {
    return rhs.opGreater(val);
  }

  virtual const Literal* opGreater(float lhs) const {
    const Literal* node;
    if(lhs >val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opGreater(int lhs) const {
    const Literal* node;
    if(lhs > val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* EqualEqual(const Literal& rhs) const {
    return rhs.opEqualEqual(val);
  }

  virtual const Literal* opEqualEqual(float lhs) const {
    const Literal* node;
    if(lhs == val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opEqualEqual(int lhs) const {
    const Literal* node;
    if(lhs == val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* GreaterEqual(const Literal& rhs) const {
    return rhs.opGreaterEqual(val);
  }

  virtual const Literal* opGreaterEqual(float lhs) const {
    const Literal* node;
    if(lhs >= val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opGreaterEqual(int lhs) const {
    const Literal* node;
    if(lhs >= val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* LessEqual(const Literal& rhs) const {
    return rhs.opLessEqual(val);
  }

  virtual const Literal* opLessEqual(float lhs) const {
    const Literal* node;
    if(lhs <= val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opLessEqual(int lhs) const {
    const Literal* node;
    if(lhs <= val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* NotEqual(const Literal& rhs) const {
    return rhs.opNotEqual(val);
  }

  virtual const Literal* opNotEqual(float lhs) const {
    const Literal* node;
    if(lhs != val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* opNotEqual(int lhs) const {
    const Literal* node;
    if(lhs != val)
      node = new IntLiteral(1);
    else
      node = new IntLiteral(0);
    PoolOfNodes::getInstance().add(node);
    return node;
  }

  virtual const Literal* MiusOp() const  {
    const Literal* node = new IntLiteral(-val);
    PoolOfNodes::getInstance().add(node);
    return node; 
  }

  virtual int isTrue() const {
    return (this->val);
  }
  
  virtual const Literal* eval() const { return this; }

  virtual void print() const { 
    std::cout << "INT: " << val << std::endl; 
  }
private:
  int val;
};

