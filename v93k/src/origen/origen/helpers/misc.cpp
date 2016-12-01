/** @file */ 
#include "../helpers.hpp"
#include <cerrno>
#include <stdlib.h>
#include "../../origen.hpp"

using namespace std;

namespace Origen {


uint64_t flip(uint64_t input, int size) {
	uint64_t output = 0;
	for (int i = 0; i < size; i++) {
		output <<= 1;
		output |= input & 1;
		input >>= 1;
	}
	return output;
}


/// Split the given string by the given delimiter and return the results in a vector of strings
///
///   split("AB1234-24", '-')   // => ["AB1234", "24"]
vector<string> split(const string &str, char delim) {
    vector<string> elems;
    split(str, delim, elems);
    return elems;
}

/// This version places the result in the supplied vector rather than returning a new one
void split(const string &str, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(str);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

/// Convert the given string to a 64-bit integer. Works for both decimal and hex strings as
/// shown in the examples below.
///
/// If the string is not successfully converted the site in focus will be binned out.
///
///   toInt("0xFF")         // => 255
///   toInt("FF", 16)       // => 255 (must declare base 16 if no leading 0x)
///   toInt("255")          // => 255
int64_t toInt (const string &str, int base)
{
    char *end;
    char *cstr = const_cast<char*>(str.c_str());
    long long int l;
    errno = 0;
    l = strtoll(cstr, &end, base);
    if ((errno == ERANGE && l == LLONG_MAX) || l > LLONG_MAX) {
        cout << "ERROR: String conversion overflowed a 64-bit integer - " << str << endl;
        ERROR_EXIT(TM::EXIT_FLOW);
    }
    if ((errno == ERANGE && l == LLONG_MIN) || l < LLONG_MIN) {
        cout << "ERROR: String conversion underflowed a 64-bit integer - " << str << endl;
        ERROR_EXIT(TM::EXIT_FLOW);
    }
    if (*cstr == '\0' || *end != '\0') {
        cout << "ERROR: String is not convertible to a 64-bit integer - " << str << endl;
        ERROR_EXIT(TM::EXIT_FLOW);
    }
    return (int64_t) l;
}

/// See toInt, but returns an unsigned 64-bit integer
uint64_t toUInt (const string &str, int base)
{
    char *end;
    char *cstr = const_cast<char*>(str.c_str());
    unsigned long long int l;
    errno = 0;
    l = strtoull(cstr, &end, base);
    if (errno == ERANGE && l == ULLONG_MAX) {
        cout << "ERROR: String conversion was out of the range of a UInt64 - " << str << endl;
        ERROR_EXIT(TM::EXIT_FLOW);
    }
    if (*cstr == '\0' || *end != '\0') {
        cout << "ERROR: String is not convertible to a UInt64 - " << str << endl;
        ERROR_EXIT(TM::EXIT_FLOW);
    }
    return (uint64_t) l;
}

/// Convert the given number to a hex string
///
///   toHex(255);    // => "ff"
string toHex (const uint64_t &val)
{
    stringstream stream;
    stream << hex << val;
    return stream.str();
}

/// Convert the given number to a string
///
///   toStr(255);    // => "255"
string toStr (const uint64_t &val)
{
    stringstream stream;
    stream << val;
    return stream.str();
}

/// Uppercase the given string
///
///   upcase("ff");  // => "FF"
string upcase(string base) {
	transform(base.begin(), base.end(), base.begin(), ::toupper);
	return base;
}

/// Extend the string to the given size, padding with the given character on the left
///
///   lpad("FF", 4, '0');  // => "00FF"
string lpad(string base, int length, char fill) {
	stringstream padded;
	padded << right << setw(length) << setfill(fill) << base;
	return padded.str();
}

/// Extend the string to the given size, padding with the given character on the right
///
///   rpad("FF", 4, ' ');  // => "FF  "
string rpad(string base, int length, char fill) {
	stringstream padded;
	padded << left << setw(length) << setfill(fill) << base;
	return padded.str();
}

/// Returns true if the given number is even
bool isEven(uint64_t number) {
	return number % 2 == 0;
}

/// Returns true if the given number is odd
bool isOdd(uint64_t number) {
	return number % 2 != 0;
}

/// Overlays the given data on the given pin, starting from the first vector of the given pattern
void overlaySubroutine(string subroutinePattern, string pin, uint64_t data, int size) {
    string p = extractPinsFromGroup(pin);
    string pat = subroutinePattern;
	VEC_LABEL_EDIT ov(pat, p);

	// Need to use a vector here so the size can be determined at runtime
	vector<VECTOR_DATA> vecData(size);

	for(int i = 0; i < size; i++) {
		int val = (data & (1 << i)) >> i;
		VECTOR_DATA v = {i, val};
		vecData[i] = v;
	}

	// However the downloadUserVectors function only accepts an array, so use this trick
	// to create an array instance that points to the vector data
	VECTOR_DATA * vecDataArray = &vecData[0];

	ov.downloadUserVectors(vecDataArray, size);
}

/// Same as overlaySubroutine but the data is applied in reverse order
void reverseOverlaySubroutine(string subroutinePattern, string pin, uint64_t data, int size) {
    string p = extractPinsFromGroup(pin);
    string pat = subroutinePattern;
	VEC_LABEL_EDIT ov(pat, p);

	// Need to use a vector here so the size can be determined at runtime
	vector<VECTOR_DATA> vecData(size);

	for(int i = 0; i < size; i++) {
	    int j = size - 1 - i;
		int val = (data & (1 << j)) >> j;
		VECTOR_DATA v = {i, val};
		vecData[i] = v;
	}

	// However the downloadUserVectors function only accepts an array, so use this trick
	// to create an array instance that points to the vector data
	VECTOR_DATA * vecDataArray = &vecData[0];

	ov.downloadUserVectors(vecDataArray, size);
}

/// Returns the number of 1's (bits that are set) in the given 32-bit number
int numberOfOnes(uint32_t i)
{
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}


double calculateFrequency(const ARRAY_I &captureData, double periodInNs) {
    int	iFreqCount;
    double dFreq_MHz;

    iFreqCount=0;
    for(int j=0; j<captureData.size()-1; j++)
    {
        if((captureData[j+1] - captureData[j]) == 1){
            iFreqCount++;
        }
    }
    dFreq_MHz = iFreqCount / ((periodInNs / 1000) * captureData.size());
    return dFreq_MHz * 1000000;
}

double calculatePeriod(const ARRAY_I &captureData, double periodInNs) {
    return 1 / calculateFrequency(captureData, periodInNs);
}

/// Call to wait for all SMC threads to complete, supply the max amount of seconds to wait
void synchronize(double timeout)
{
    bool ok;
    int site = CURRENT_SITE_NUMBER();

    if (timeout) {
        ok = SMC_SYNCHRONIZE(timeout s);
    } else {
        ok = SMC_SYNCHRONIZE();
    }

    if (! ok) {
        cout << "ERROR: All SMC threads not complete after " << timeout << " second(s) on site " << site << endl;
        ERROR_EXIT(TM::EXIT_FLOW);
    }
}

/// Supply a pin alias name and get the name of the physical pin returned
string extractPinsFromGroup(const string& groupname)
{
    string pinlist;

    try{
        pinlist = PinUtility.createPinListFromPinNames(PinUtility.getDigitalPinNamesFromPinList(groupname, TM::ALL_DIGITAL,true,true,PIN_UTILITY::DEFINITION_ORDER));
    } catch(...) {
        cout << "ERROR: Pingroup " << groupname << " not found." << endl;
        ERROR_EXIT(TM::ABORT_FLOW);
    }

    return pinlist;
}

// Initializes the site objects that are used to store the ECID, this should be called at
// the start of the test flow if using the Origen.site API
void initializeSites() {
    INT num_of_sites;
    ARRAY_I sites;

    ON_FIRST_INVOCATION_BEGIN();

        num_of_sites = GET_CONFIGURED_SITES(sites);

        // Leave an empty space at 0, easier then to fetch by site number (which start at 1)
        Origen::Sites.resize(num_of_sites + 1, Origen::Site(0));

    ON_FIRST_INVOCATION_END();

    // Instantiate a new site object and save it in the global site array
    Origen::Site site(CURRENT_SITE_NUMBER());

    Origen::Sites[CURRENT_SITE_NUMBER()] = site;
}

}
