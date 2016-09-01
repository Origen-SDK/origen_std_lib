#include "../helpers.hpp"

using namespace std;

namespace Origen {

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

int autorange(const LIMIT& limits)
{
//    double * low;
//    double * high;
//    string units = limits.unit();
//    TM::COMPARE compare;
//    compare = limits.getLow(low);
//    compare = limits.getHigh(high);
//
//    double abs_limit;
//
//    if (abs(*high) > abs(*low))
//        abs_limit = abs(*high);
//    else
//        abs_limit = abs(*low);
//
//    cout << "LIMIT*****************:" << abs_limit << endl;
//
//    if(     units == "nA") abs_limit *= 1E-3;
//    else if(units == "uA") abs_limit *= 1;
//    else if(units == "mA") abs_limit *= 1E3;
//    else if(units == "A" ) abs_limit *= 1E6;
//    else {
//        //Can't determine units, issue warning and set clamp to max
//        cout << "ERROR! Can determine units, setting clamp to 40mA" << endl;
//        return 40000;
//    }
//
//    if(      abs_limit <= 2    ) return 2;
//    else if( abs_limit <= 10   ) return 10;
//    else if( abs_limit <= 100  ) return 100;
//    else if( abs_limit <= 1000 ) return 1000;
//    else                         return 40000;
    return 10;
}


int autorange(const double& forceValue)
{
    int range;

    if(     abs(forceValue) <= 2   ) range = 2;
    else if(abs(forceValue) <= 10  ) range = 10;
    else if(abs(forceValue) <= 100 ) range = 100;
    else if(abs(forceValue) <= 1000) range = 1000;
    else                             range = 400000;

    return range;
}
}
