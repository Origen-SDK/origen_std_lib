#include "site.hpp"
#include <libcicpi.h>
#include <iostream>
#include "helpers.hpp"
using namespace std;

namespace Origen {

Site::Site(int number) {
    _number = number;
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

 	if (id.length() > 8) {
 		if (id == "Undefined") {
 			id = "Undefine";
 		} else {
 			cout << "ERROR: Lot ID is greater than 8 characters and cannot be converted to a UInt64: " << id << endl;
 			ERROR_EXIT(TM::EXIT_FLOW);
 		}
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
uint8_t Site::wafer() {
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
int16_t Site::x() {
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
int16_t Site::y() {
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


} /* namespace Origen */
