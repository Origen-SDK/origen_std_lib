#ifndef ORIGEN_HELPERS_INCLUDED
#define ORIGEN_HELPERS_INCLUDED

#include "mapi.hpp"
#include "rdi.hpp"
#include <string>

using namespace std;

namespace Origen {

string extractPinsFromGroup(const string&);
void synchronize(double = 1);
double calculateFrequency(const ARRAY_I&, double);
double calculatePeriod(const ARRAY_I&, double);
void overlaySubroutine(string, string, uint64_t, int);
int numberOfOnes(uint32_t);
int toInt (const string&, int = 0);
uint64_t toUInt64 (const string&, int = 0);
vector<string> split(const string&, char);
void split(const string&, char, vector<string>&);
string toHex (const int&);
void initializeSites();

}

#endif
