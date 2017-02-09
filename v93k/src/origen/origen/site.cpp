#include "site.hpp"
#include <libcicpi.h>
#include <iostream>
#include "helpers.hpp"
using namespace std;

namespace Origen {

Site::Site(int number) {
    _number = number;
    lotidSet = false;
    waferSet = false;
    xSet = false;
    ySet = false;
    binSet = false;
    softbinSet = false;
}
Site::~Site() {}

/// Set the lot ID to the given value, e.g. "ABC1234"
void Site::lotid(string val) {
    lotidSet = true;
    _lotid = val;
}


/// Set the lot ID based on the integer representation returned from lotidInt
void Site::lotid(uint64_t val) {
	string id = "";

	for(int i = 0; i < 8; i++) {
		int ch = val >> (8 * (7 - i));
		if (ch != 0) {
			id = id + (char)ch;
		}
	}

	lotid(id);
}


/// Get the lot ID. If it has not previously been set to a value it will be automatically queried from the test system.
string Site::lotid() {
    if (!lotidSet) {
        char value[CI_CPI_MAX_MODL_STRING_LEN * 2];
        if (!GetModelfileString(const_cast<char*>("LOT_ID"), value)) {
            _lotid = (string) value;
        } else {
            _lotid = "Undefined";
        }
        lotidSet = true;
    }
    return _lotid;
}

/// Returns the lot ID as a 64-bit integer that is suitable for programming to the device.
/// Each character in the lotID is converted to its ASCII code.
/// An error will be raised if the length of the current lotID overflows 64-bits.
uint64_t Site::lotidInt() {
	string id = lotid();
 	stringstream val;

	// If the ID is > 8 chars then lose the upper chars, making the assuming that the lower ones are
	// the more significant ones for the purposes of identifying a particular lot
 	if (id.length() > 8) {
		id = id.substr(id.length() - 8, 8);
 	}
 	if (id.length() > 0) val << toHex((int)id.at(0));
 	if (id.length() > 1) val << toHex((int)id.at(1));
 	if (id.length() > 2) val << toHex((int)id.at(2));
 	if (id.length() > 3) val << toHex((int)id.at(3));
 	if (id.length() > 4) val << toHex((int)id.at(4));
 	if (id.length() > 5) val << toHex((int)id.at(5));
 	if (id.length() > 6) val << toHex((int)id.at(6));
 	if (id.length() > 7) val << toHex((int)id.at(7));

 	return toUInt(val.str(), 16);
}


/// Set the wafer number to the given value
void Site::wafer(int val) {
    waferSet = true;
    if (val < 0 || val > 255) {
        cout << "ERROR: Wafer is out of the range of a UInt8: " << val << endl;
        ERROR_EXIT(TM::EXIT_FLOW);
    }
    _wafer = (uint8_t)val;
}

/// Get the wafer number. If it has not previously been set to a value it will be automatically queried from the test system.
int Site::wafer() {
    if (!waferSet) {
        char value[CI_CPI_MAX_MODL_STRING_LEN * 2];

//      if (!GetModelfileString(const_cast<char*>("WAFER_ID"), value)) {
            // Expect to return something like "AB1234-15AA", where 15 is the wafer number
            //wafer(toInt(split((string) value, '-')[1].substr(0, 2)));
        if (!GetModelfileString(const_cast<char*>("WAFER_NUMBER"), value)) {
        	wafer(toInt((string) value));
        } else {
            wafer(0xFF);
        }
        waferSet = true;
    }
    return _wafer;
}


/// Set the X co-ordinate to the given value
void Site::x(int val) {
    xSet = true;
    _x = val;
}

/// Get the X co-ord. If it has not previously been set to a value it will be automatically queried from the test system.
int Site::x() {
    if (!xSet) {
        long lx, ly;
        GetDiePosXYOfSite(_number, &lx, &ly);
     	if (lx < -32768 || lx > 32767) {
            cout << "ERROR: X is out of the range of an Int16: " << lx << endl;
            ERROR_EXIT(TM::EXIT_FLOW);
     	}
     	if (ly < -32768 || ly > 32767) {
            cout << "ERROR: Y is out of the range of an Int16: " << ly << endl;
            ERROR_EXIT(TM::EXIT_FLOW);
     	}
        _x = (int) lx;
        _y = (int) ly;
        xSet = true;
        ySet = true;
    }
    return _x;
}


/// Set the Y co-ordinate to the given value
void Site::y(int val) {
    ySet = true;
    _y = val;
}

/// Get the Y co-ord. If it has not previously been set to a value it will be automatically queried from the test system.
int Site::y() {
    if (!ySet) {
        long lx, ly;
        GetDiePosXYOfSite(_number, &lx, &ly);
     	if (lx < -32768 || lx > 32767) {
            cout << "ERROR: X is out of the range of an Int16: " << lx << endl;
            ERROR_EXIT(TM::EXIT_FLOW);
     	}
     	if (ly < -32768 || ly > 32767) {
            cout << "ERROR: Y is out of the range of an Int16: " << ly << endl;
            ERROR_EXIT(TM::EXIT_FLOW);
     	}
        _x = (int) lx;
        _y = (int) ly;
        xSet = true;
        ySet = true;
    }
    return _y;
}

/// Set the site's bin to the given value, but only if a bin has not already been set.
/// Note that this does not actually bin out the site but just records the bin assignment
/// in a variable that can be retrieved by calling bin().
void Site::bin(int val) {
	if (!binSet) {
		binSet = true;
		_bin = val;
	}
}

/// Set the site's bin to the given value, but only if a bin has not already been set or
/// if the force argument is set to true, in which case it will overwrite any previous
/// assignment.
/// Note that this does not actually bin out the site but just records the bin assignment
/// in a variable that can be retrieved by calling bin().
void Site::bin(int val, bool force) {
	if (!binSet || force) {
		binSet = true;
		_bin = val;
	}
}

/// Returns the bin that has been assigned to the site by previously calling bin(int), if no
/// bin has been assigned then it will return 0.
int Site::bin() {
	return (binSet) ? _bin : 0;
}

/// Set the site's softbin to the given value, but only if a softbin has not already been set.
/// Note that this does not actually bin out the site but just records the softbin assignment
/// in a variable that can be retrieved by calling softbin().
void Site::softbin(int val) {
	if (!softbinSet) {
		softbinSet = true;
		_softbin = val;
	}
}

/// Set the site's softbin to the given value, but only if a softbin a has not already been set or
/// if the force argument is set to true, in which case it will overwrite any previous
/// assignment.
/// Note that this does not actually bin out the site but just records the softbin assignment
/// in a variable that can be retrieved by calling softbin().
void Site::softbin(int val, bool force) {
	if (!softbinSet || force) {
		softbinSet = true;
		_softbin = val;
	}
}

/// Returns the bin that has been assigned to the site by previously calling bin(int), if no
/// bin has been assigned then it will return 0.
int Site::softbin() {
	return (softbinSet) ? _softbin : 0;
}
} /* namespace Origen */
