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
void reverseOverlaySubroutine(string, string, uint64_t, int);
int numberOfOnes(uint32_t);
int64_t toInt (const string&, int = 0);
uint64_t toUInt (const string&, int = 0);
vector<string> split(const string&, char);
void split(const string&, char, vector<string>&);
string toHex (const uint64_t&);
string toStr (const uint64_t&);
void initializeSites();
void logParametricTest(string, int, double, LIMIT, string);
void logFunctionalTest(string, int, bool, string);
bool isPass(double, LIMIT);
string loStr(LIMIT);
string hiStr(LIMIT);
string upcase(string);
string lpad(string, int, char);
string rpad(string, int, char);
bool isEven(uint64_t);
bool isOdd(uint64_t);
LIMIT noLimits();
uint64_t flip(uint64_t, int);

}

#endif
