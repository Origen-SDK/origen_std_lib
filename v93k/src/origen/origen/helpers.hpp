#ifndef ORIGEN_HELPERS_INCLUDED
#define ORIGEN_HELPERS_INCLUDED

#include "mapi.hpp"
#include "rdi.hpp"
#include <string>

using namespace std;

namespace Origen {

string raise(string errorMessage);
string extractPinsFromGroup(const string& groupname);
int autorange(const LIMIT& limit);
int autorange(const double& forceValue);

}


#endif
