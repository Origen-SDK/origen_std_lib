#ifndef ORIGEN_HELPERS_INCLUDED
#define ORIGEN_HELPERS_INCLUDED

#include "mapi.hpp"
#include "rdi.hpp"
#include <string>

using namespace std;

namespace Origen {

string extractPinsFromGroup(const string&);
bool pinExists(const string& pinName);
void synchronize(double = 1);
double calculateFrequency(const ARRAY_I&, double);
double calculatePeriod(const ARRAY_I&, double);
void overlaySubroutineOutput(string, string, uint64_t, int, int, int);
void overlaySubroutine(string, string, uint64_t, int);
void reverseOverlaySubroutine(string, string, uint64_t, int);
int numberOfOnes(uint32_t);
int numberOfZeros(uint32_t);
int64_t toInt (string, int = 0);
uint64_t toUInt (string, int = 0);
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
string downcase(string);
string lpad(string, int, char);
string rpad(string, int, char);
bool isEven(uint64_t);
bool isOdd(uint64_t);
LIMIT noLimits();
uint64_t flip(uint64_t, int);
string ltrim(string, const char * remove_chars = " \n\t\r\f\v");
string rtrim(string, const char * remove_chars = " \n\t\r\f\v");
string trim(string, const char * remove_chars = " \n\t\r\f\v");
double getUnitMultiplier(const string& units);
string getLabelFromBurst(const string& burst, const string& port);
string checksum16bits(const string&);

namespace Time {
  void initTime(bool = false);
  int month();
  int day();
  int hour();
  int minute();
  int second();
}

}

#endif
