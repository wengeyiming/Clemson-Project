#include <map>
#include <algorithm>
#include "tableManager.h"
#include "symbolTable.h"
#include "functionTable.h"
#include "literal.h"

TableManager& TableManager::getInstance()
{
   static TableManager instance;
   return instance;
}

const Literal* TableManager::getLiteral(const std::string & name)
{
   std::vector<SymbolTable>::reverse_iterator rit=tables.rbegin();
   for(; rit !=tables.rend(); ++rit)
   {
      if(rit->isFound(name)) 
      return rit->getLiteral(name);
   }
   if(rit == tables.rend()) throw name+std::string(" not found");
   return 0;
} 

const Node* TableManager::getSuite(const std::string & name)
{
   std::vector<FunctionTable>::reverse_iterator rit = functions.rbegin();
   for(; rit != functions.rend(); ++rit)
   {
     if (rit->isFound(name))
       return rit->getSuite(name);
   }
   if(rit == functions.rend()) throw name+std::string("not found");
   return 0;
} 

void TableManager::insert(const std::string& name, const Literal* node)
{
   tables[currentScope].insertLiteral(name,node);
}

void TableManager::insert(const std::string& name, const Node* node)
{
   functions[currentScope].insertSuite(name,node);
}
bool TableManager::checkName(const std::string& name) const
{
   return tables[currentScope].isFound(name);
}
bool TableManager::checkFunction(const std::string& name) const
{
   return functions[currentScope].isFound(name);
}

void TableManager::pushScope()
{ 
   currentScope++;
   FunctionTable ftable;
   SymbolTable stable;
   tables.push_back(stable);
   functions.push_back(ftable);
}
void TableManager::popScope()
{
   tables.pop_back();
   functions.pop_back();
   currentScope--;
}
