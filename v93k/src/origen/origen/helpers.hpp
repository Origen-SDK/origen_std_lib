#ifndef ORIGEN_HELPERS_INCLUDED
#define ORIGEN_HELPERS_INCLUDED

#include "mapi.hpp"
#include "rdi.hpp"
#include <string>

using namespace std;

namespace Origen {

string extractPinsFromGroup(const string& groupname);
void synchronize(double timeout = 1);
double calculateFrequency(const ARRAY_I &captureData, double periodInNs);
double calculatePeriod(const ARRAY_I &captureData, double periodInNs);

}

#endif
