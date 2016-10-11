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

/// Set the lot ID to the given value
void Site::lotid(string val) {
    lotidSet = true;
    _lotid = val;
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

/// Set the wafer number to the given value
void Site::wafer(int val) {
    waferSet = true;
    _wafer = val;
}

/// Get the wafer number. If it has not previously been set to a value it will be automatically queried from the test system.
int Site::wafer() {
    if (!waferSet) {
        char value[CI_CPI_MAX_MODL_STRING_LEN * 2];
        if (!GetModelfileString(const_cast<char*>("WAFER_ID"), value)) {
            // Expect to return something like "AB1234-15AA", where 15 is the wafer number
            _wafer = toInt(split((string) value, '-')[1].substr(0, 2));
        } else {
            _wafer = -9999;
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
        _x = (int) lx;
        _y = (int) ly;
        xSet = true;
        ySet = true;
    }
    return _y;
}


} /* namespace Origen */
