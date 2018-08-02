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

const Literal* TableManager::getEntry(const std::string & name)
{
  std::vector<SymbolTable>::reverse_iterator rit=tables.rbegin();
  for(;rit != tables.rend(); ++rit)
   {
      if(rit->found(name)) 
      {
         tables[currentScope].insert(name,rit->getEntry(name));
         return rit->getEntry(name);
      }
   }

  if(rit == tables.rend() ) throw name+std::string("not found");
  return 0;
} 

const Node* TableManager::getSuite(const std::string & name)
{
   std::vector<FunctionTable>::reverse_iterator rit = functions.rbegin();
   for(; rit != functions.rend(); ++rit)
   {
      if (rit->found(name))
         return rit->getEntry(name);
   }
   if(rit == functions.rend()) throw name+std::string("not found");

   return 0;
} 

const Node* TableManager::getParameter(const std::string& name)
{
   std::vector<FunctionTable>::const_reverse_iterator it = parameters.rbegin();
   while(it != parameters.rend())
   {
      if(it->found(name))
         return it->getEntry(name);
      it++;
   }
   if(it == parameters.rend()) throw name + std::string("not found");
   return 0;
}

void  TableManager::insert(const std::string& name, const Literal* node)
{
   tables[currentScope].insert(name,node);
}

void  TableManager::insert(const std::string& name, const Node* node)
{
   functions[currentScope].insert(name,node);
}

void TableManager::insertargs(const std::string& name, const Node* node)
{
   parameters[currentScope].insert(name,node);
}


bool  TableManager::checkName(const std::string& name) const
{  
   std::vector<SymbolTable>::const_reverse_iterator rit = tables.rbegin();
   for(; rit != tables.rend();rit++)
   {
      if(rit->found(name))
         return true;
   }
   return false;
}
bool  TableManager::checkFunc(const std::string& name) const
{   
   std::vector<FunctionTable>::const_reverse_iterator rit = functions.rbegin();
   for(; rit != functions.rend();rit++)
   {
      if(rit->found(name))
         return true;
   }
   return false;
   return 0;
}

bool TableManager::checkCurrentName(const std::string& name) const
{
   std::vector<SymbolTable>::const_reverse_iterator rit = tables.rbegin();
   if(rit->found(name)) return true;
   else 
      return false;
}

void TableManager::pushScope(const std::string& name)
{ 
   currentScope++;
   FunctionTable ftable,argus;
   SymbolTable stable;

   if(checkFuncScope(name) != currentScope)
   {
      stable = tables[checkFuncScope(name)-1];
   }
   if(checkFunc(name))
   {
      ftable.insert(name,getSuite(name));
      argus.insert(name,getParameter(name));
   }
   tables.push_back(stable);
   functions.push_back(ftable);
   parameters.push_back(argus);
}

void TableManager::popScope()
{
   tables.pop_back();
   functions.pop_back();
   parameters.pop_back();
   currentScope--;
}


int TableManager::checkFuncScope(const std::string& name) const
{
   std::vector<FunctionTable>::const_reverse_iterator rit = functions.rbegin();
   int res = functions.size();
   while( rit != functions.rend())
   {
      if(rit->found(name))
      {
         return res;
      }
      else
         res--;

      ++rit;
   }
   if(rit == functions.rend()) throw std::string("can't find scope");
   return 0;
}