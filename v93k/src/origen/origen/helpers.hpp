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
void overlaySubroutine(string subroutinePattern, string pin, int data, int size);
int numberOfOnes(uint32_t i);
int toInt (const string &str, int base = 0);
vector<string> split(const string &str, char delim);
void split(const string &str, char delim, vector<string> &elems);
void initializeSites();

}

#endif
