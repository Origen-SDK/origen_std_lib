/** @file */ 
#include "../helpers.hpp"

using namespace std;

namespace Origen {

// Overlays the given data on the given pin, starting from the first vector of the given
// pattern.
void overlaySubroutine(string subroutinePattern, string pin, int data, int size) {

	VEC_LABEL_EDIT ov(subroutinePattern, extractPinsFromGroup(pin));

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

}
