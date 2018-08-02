#include <map>
#include <algorithm>
#include "literal.h"
#include "symbolTable.h"

bool SymbolTable::isFound(const std::string& name) const
{
   std::map<std::string, const Literal*>::const_iterator it = table.find(name);
   if (it != table.end()) return true;
   return false;

}
void SymbolTable::insertLiteral(const std::string& name, const Literal* val)
{
   table[name]=val;
}
const Literal* SymbolTable::getLiteral(const std::string& name) const
{
   std::map<std::string, const Literal*>::const_iterator it = table.find(name);
   if ( it == table.end() ) throw name+std::string(" not found");
   return it->second;
}
