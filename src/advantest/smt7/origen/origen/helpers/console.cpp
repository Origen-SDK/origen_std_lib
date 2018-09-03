/** @file */
#include "../helpers.hpp"

namespace Origen {

/// Prints the result of a parametric test to the console
void logParametricTest(string name, int site, double result, LIMIT limits, string pin) {

    cout << site << " ";
    if (isPass(result, limits))
        cout << "     ";
    else
        cout << "FAIL ";
    cout << left << setw(65) << setfill(' ') << name << "  ";
    cout << left << setw(20) << setfill(' ') << loStr(limits) << "  ";
    cout << left << setw(20) << setfill(' ') << result;
    cout << left << setw(20) << setfill(' ') << hiStr(limits);
    cout << pin << endl;

}

/// Returns a LIMIT object with no limits enabled
LIMIT noLimits() {
	return LIMIT(TM::NA, 0, TM::NA, 0);
}

/// Prints the result of a functional test to the console
void logFunctionalTest(string name, int site, bool passed, string pattern) {
    cout << site << " ";
    if (passed)
        cout << "     ";
    else
        cout << "FAIL ";
    cout << left << setw(65) << setfill(' ') << name << "  ";
    cout << pattern << endl;
}

/// Converts the low limit in the given LIMIT object into a string, where an empty string will be returned if the limit is not enabled
string loStr(LIMIT limits) {
    double lo = 0.0;
    double * plo = &lo;
    TM::COMPARE clo;

    clo = limits.getLow(plo);

    if (clo == TM::NA) {
        return "";
    } else {
        stringstream val;
        val << lo;
        return val.str();
    }
}

/// Converts the high limit in the given LIMIT object into a string, where an empty string will be returned if the limit is not enabled
string hiStr(LIMIT limits) {
    double hi = 0.0;
    double * phi = &hi;
    TM::COMPARE chi;

    chi = limits.getHigh(phi);

    if (chi == TM::NA) {
        return "";
    } else {
        stringstream val;
        val << hi;
        return val.str();
    }
}
/// Returns true if the result is within the given limits
bool isPass(double val, LIMIT limits) {
    double lo = 0.0;
    double hi = 0.0;
    double * plo = &lo;
    double * phi = &hi;
    TM::COMPARE clo, chi;
    bool result;

    clo = limits.getLow(plo);
    chi = limits.getHigh(phi);

    result = true;

    if (clo != TM::NA) {
        if (clo == TM::GE && val < lo)
            result = false;
        if (clo == TM::GT && val <= lo)
            result = false;
    }
    if (chi != TM::NA) {
        if (chi == TM::LE && val > hi)
            result = false;
        if (chi == TM::LT && val >= hi)
            result = false;
    }
    return result;
}

}
