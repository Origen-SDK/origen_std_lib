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

///
string ltrim(string str, const char * remove_chars /* = " \n\t\r\f\v" */) {
	str.erase(0, str.find_first_not_of(remove_chars));
	return str;
}


string rtrim(string str, const char * remove_chars /* = " \n\t\r\f\v" */) {
	str.erase(str.find_last_not_of(remove_chars)+1);
	return str;
}


string trim(string str, const char * remove_chars /* = " \n\t\r\f\v" */) {
	str = rtrim(str, remove_chars);
	str = ltrim(str, remove_chars);
	return str;
}


/// Convert the given string to a 64-bit integer. Works for both decimal and hex strings as
/// shown in the examples below.
///
/// If the string is not successfully converted the site in focus will be binned out.
///
///   toInt("0xFF")         // => 255
///   toInt("FF", 16)       // => 255 (must declare base 16 if no leading 0x)
///   toInt("255")          // => 255
int64_t toInt (string str, int base)
{
	str = trim(str);        // Remove any whitespace
	str = ltrim(str, "0");  // Remove any leading 0's
	if (str == "") {
		return 0;
	}
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
uint64_t toUInt (string str, int base)
{
	str = trim(str);        // Remove any whitespace
	str = ltrim(str, "0");  // Remove any leading 0's
	if (str == "") {
		return 0;
	}

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

/// Lowercase the given string
///
///   downcase("FF");  // => "ff"
string downcase(string base) {
	transform(base.begin(), base.end(), base.begin(), ::tolower);
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
/// must provide the waveform indices used for output high and low
void overlaySubroutineOutput(string subroutinePattern, string pin, uint64_t data, int size, int loOutputIndex, int hiOutputIndex) {
    string p = extractPinsFromGroup(pin);
    //string pat = getTdiLabelName(subroutinePattern);
    string pat = subroutinePattern;
	VEC_LABEL_EDIT ov(pat, p);

	// Need to use a vector here so the size can be determined at runtime
	vector<VECTOR_DATA> vecData(size);

	for(int i = 0; i < size; i++) {

		int val = (data & (1 << i)) >> i;
		if (val == 1)
			val = hiOutputIndex;
		else
			val = loOutputIndex;
		VECTOR_DATA v = {i, val};
		vecData[i] = v;
	}

	// However the downloadUserVectors function only accepts an array, so use this trick
	// to create an array instance that points to the vector data
	VECTOR_DATA * vecDataArray = &vecData[0];

	ov.downloadUserVectors(vecDataArray, size);
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

/// Returns the number of 0's (bits that are not set) in the given 32-bit number
int numberOfZeros(uint32_t i)
{
     return 32 - numberOfOnes(i);
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

// Check to see if a pin exists in current pin config
bool pinExists(const string& pinName) {
    stringstream fw_command;
    string fw_answer;
    bool result;

    fw_command << "dfpn? (" << pinName << ")" << endl;

    FW_TASK(fw_command.str(), fw_answer);
    fw_command.str(std::string());

    result = !(fw_answer.empty());
    return result;
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

        Origen::Time::getTime(); // get start of flow time for later use

    ON_FIRST_INVOCATION_END();

    // Instantiate a new site object and save it in the global site array
    Origen::Site site(CURRENT_SITE_NUMBER());

    Origen::Sites[CURRENT_SITE_NUMBER()] = site;
}

double getUnitMultiplier(const string& units) {
	double mult;

	if     (units.find("nA") != string::npos)  { mult = 1e9; }
	else if(units.find("uA") != string::npos)  { mult = 1e6; }
	else if(units.find("mA") != string::npos)  { mult = 1e3; }
	else if(units.find("A") != string::npos)   { mult = 1; }
	else if(units.find("nV") != string::npos)  { mult = 1e9; }
	else if(units.find("uV") != string::npos)  { mult = 1e6; }
	else if(units.find("mV") != string::npos)  { mult = 1e3; }
	else if(units.find("V") != string::npos)   { mult = 1; }
	else if(units.find("Kohm") != string::npos){ mult = 1e-3; }
	else if(units.find("ns") != string::npos)  { mult = 1e9; }
	else if(units.find("us") != string::npos)  { mult = 1e6; }
	else if(units.find("ms") != string::npos)  { mult = 1e3; }
	else if(units.find("s") != string::npos)   { mult = 1; }
	else if(units.find("KHz") != string::npos)  { mult = 1e-3; }
	else if(units.find("MHz") != string::npos)  { mult = 1e-6; }
	else 									   { mult = 1; }

	return mult;
}

string getLabelFromBurst(const string& burst, const string& port) {

	  ////////////////////////////////////////////////////////////////////
	  // Note this function will always return the first label in a burst
	  ////////////////////////////////////////////////////////////////////
	  string answer;
	  stringstream ss;

	  ss << "sqpg? \"" << burst << "\",(" << port << ")" << endl;

	  FW_TASK(ss.str(), answer);

	  ss.str("");

	  int fc = answer.find_first_of("\"")+1;
	  int len = answer.find_last_of("\"") - fc;

	  return(answer.substr(fc, len));

}

string checksum16bits(const string& mystring) {

	long flag;
	long poly = 0xa001;
	long crc =  0xffff;
	string crc16;

	vector<char> ch(mystring.begin(), mystring.end());

	for(unsigned int i=0; i<ch.size(); i++) {

		crc ^= static_cast<int>(ch[i]);

		for(unsigned int j=0; j<8; j++) {

			flag = crc & long(0x1);
			crc /= long(0x2);

			if(flag == 1)
				crc ^= poly;
		}
	}

	unsigned n;
	stringstream ss;
	ss << hex << (crc & long(0xffff));
	ss >> n;
	ss.str("");

	bitset<16> bstr(n);

	crc16 = bstr.to_string();

	return crc16;

}

}
