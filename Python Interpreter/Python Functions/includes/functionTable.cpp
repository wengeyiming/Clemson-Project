#include <map>
#include <algorithm>
#include "functionTable.h"
#include "literal.h"

bool FunctionTable::isFound(const std::string& name) const
{
  std::map<std::string, const Node*>::const_iterator it = functions.find(name);
  if ( it == functions.end() ) return false;
  else return true;

}

void FunctionTable::insertSuite(const std::string& name, const Node* suite)
{
	functions[name]=suite;

}

const Node* FunctionTable::getSuite(const	std::string& name) const
{
  std::map<std::string, const Node*>::const_iterator it = functions.find(name);
  if ( it == functions.end() ) throw name+std::string(" not found");
  return it->second;
}
