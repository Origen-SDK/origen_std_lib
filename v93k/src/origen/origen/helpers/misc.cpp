/** @file */ 
#include "../helpers.hpp"

using namespace std;

namespace Origen {

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
