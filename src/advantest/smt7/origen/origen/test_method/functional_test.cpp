#include "functional_test.hpp"

namespace Origen {
namespace TestMethod {

// Defaults
FunctionalTest::FunctionalTest() {
    pin("");
    port("");
    capture(0);
    processResults(1);
    bitPerWord(1);
    pattern("");
    testName("");
}

FunctionalTest::~FunctionalTest() { }

/// The functional test method can optionally capture data by supplying the number of vectors to capture
FunctionalTest & FunctionalTest::capture(int v) { _capture = v; return *this; }
/// If data capture is requested, supply the pin to capture data from
FunctionalTest & FunctionalTest::pin(string v) { _pin = v; return *this; }
/// Optionally supply the name of the test port that should be used to execute the pattern
FunctionalTest & FunctionalTest::port(string v) { _port = v; return *this; }
/// Serial capture data will be grouped into words, specify how many bits per word in the serial stream (default 1)
FunctionalTest & FunctionalTest::bitPerWord(int v) { _bitPerWord = v; return *this; }
/// When set to 0 the results of the test will not be judged or logged
FunctionalTest & FunctionalTest::processResults(int v) { _processResults = v; return *this; }
/// Override the pattern argument from the test suite
FunctionalTest & FunctionalTest::pattern(string v) { _pattern = v; return *this; }
/// Override the test name argument from the test suite, this can be useful if the main test item to be logged is
/// a captured value and the pattern execution is to be logged separately. The override is only valid for one call to execute()
/// and then it will be removed.
FunctionalTest & FunctionalTest::testName(string v) { _testNameOverride = v; return *this; }

// All test methods must implement this function
FunctionalTest & FunctionalTest::getThis() { return *this; }

void FunctionalTest::_setup() {
	results.resize(numberOfPhysicalSites + 1);
}

void FunctionalTest::_execute() {

    int site;

    ON_FIRST_INVOCATION_BEGIN();

    if (_pattern.empty()) {
    	label = Primary.getLabel();
    } else {
    	label = _pattern;
    }

    if (_capture) {
        pinName = extractPinsFromGroup(_pin);
    }

    RDI_BEGIN();

        if (_capture) {
            if (_port.empty()) {
              SMART_RDI::DIG_CAP & prdi = rdi.digCap(suiteName)
                                             .label(label)
                                             .pin(pinName)
                                             .bitPerWord(_bitPerWord)
                                             .samples(_capture);
              filterRDI(prdi);
              prdi.execute();

            } else {
              SMART_RDI::DIG_CAP & prdi = rdi.port(_port).digCap(suiteName)
                                             .vecVarOnly()
                                             .pin(pinName)
                                             .bitPerWord(_bitPerWord)
                                             .samples(_capture);
              filterRDI(prdi);
              prdi.execute();
              rdi.port(_port).func().burst(label).execute();
            }

        } else {
            if (_port.empty()) {
              SMART_RDI::FUNC & prdi = rdi.func(suiteName).label(label);
            filterRDI(prdi);
            prdi.execute();            
	    } else {
              SMART_RDI::FUNC & prdi = rdi.port(_port).func(suiteName).burst(label);
            filterRDI(prdi);
            prdi.execute();		    
            }
        }

    RDI_END();

    FOR_EACH_SITE_BEGIN();
        site = CURRENT_SITE_NUMBER();
        if (_capture) {
            results[site] = rdi.site(site).getBurstPassFail();
//        	cout << "PRE " << site << ": " << results[site] << endl;

        } else {
            results[site] = rdi.site(site).id(suiteName).getPassFail();
//            cout << "PRE " << site << ": " << results[site] << endl;
        }
    FOR_EACH_SITE_END();

    ON_FIRST_INVOCATION_END();
}

/// Returns the captured data for the site currently in focus
ARRAY_I FunctionalTest::capturedData() {
    return rdi.id(suiteName).getVector(pinName);
}

/// Returns the captured data for the given site number
ARRAY_I FunctionalTest::capturedData(int site) {
    return rdi.site(site).id(suiteName).getVector(pinName);
}

void FunctionalTest::serialProcessing(int site) {
	if (_processResults) {
		if (_testNameOverride != "") {
			judgeAndDatalog(_testNameOverride, invertFunctionalResultIfRequired(results[site]));
		} else {
			judgeAndDatalog(invertFunctionalResultIfRequired(results[site]));
		}
	}
	_testNameOverride = "";
}

void FunctionalTest::SMC_backgroundProcessing() {
	for (int i = 0; i < activeSites.size(); i++) {
		int site = activeSites[i];
		process(site);
		if (_processResults) {
			SMC_TEST(site, "", suiteName, LIMIT(TM::GE, 1, TM::LE, 1), results[activeSites[i]]);
		}
	}
}

}
}
