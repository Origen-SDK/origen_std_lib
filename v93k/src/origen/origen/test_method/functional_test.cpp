#include "functional_test.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {


FunctionalTest::FunctionalTest() {
    pin("");
    capture(0);
    processResults(1);
    bitPerWord(1);
}

FunctionalTest::~FunctionalTest() { }

/// The functional test method can optionally capture data by supplying the number of vectors to capture
FunctionalTest & FunctionalTest::capture(int v) { _capture = v; return *this; }
/// If data capture is requested, supply the pin to capture data from
FunctionalTest & FunctionalTest::pin(string v) { _pin = v; return *this; }
/// Serial capture data will be grouped into words, specify how many bits per word in the serial stream (default 1)
FunctionalTest & FunctionalTest::bitPerWord(int v) { _bitPerWord = v; return *this; }
/// When set to 0 the results of the test will not be judged or logged
FunctionalTest & FunctionalTest::processResults(int v) { _processResults = v; return *this; }

// All test methods must implement this function
FunctionalTest & FunctionalTest::getThis() { return *this; }

void FunctionalTest::execute() {

    int site, physicalSites;
    ARRAY_I sites;

    RDI_INIT();

    ON_FIRST_INVOCATION_BEGIN();

    enableHiddenUpload();
    GET_ACTIVE_SITES(activeSites);
    physicalSites = GET_CONFIGURED_SITES(sites);
    results.resize(physicalSites + 1);
    GET_TESTSUITE_NAME(testSuiteName);
    label = Primary.getLabel();

    if (_capture) {
        pinName = extractPinsFromGroup(_pin);
    }

    callPreTestFunc();

    RDI_BEGIN();

        if (_capture) {
            SMART_RDI::DIG_CAP & prdi = rdi.digCap(testSuiteName)
                                           .label(label)
                                           .pin(pinName)
                                           .bitPerWord(_bitPerWord)
                                           .samples(_capture);
            filterRDI(prdi);
            prdi.execute();

        } else {
            SMART_RDI::FUNC & prdi = rdi.func(testSuiteName).label(label);
            filterRDI(prdi);
            prdi.execute();
        }

    RDI_END();

    FOR_EACH_SITE_BEGIN();
        site = CURRENT_SITE_NUMBER();
        if (_capture)
            results[site] = rdi.getBurstPassFail();
        else
            results[site] = rdi.id(testSuiteName).getPassFail();
    FOR_EACH_SITE_END();

    ON_FIRST_INVOCATION_END();

    callPostTestFunc(this);
}

/// Returns the captured data for the site currently in focus
ARRAY_I FunctionalTest::capturedData() {
    return rdi.id(testSuiteName).getVector(pinName);
}

/// Returns the captured data for the given site number
ARRAY_I FunctionalTest::capturedData(int site) {
    return rdi.site(site).id(testSuiteName).getVector(pinName);
}

void FunctionalTest::serialProcessing(int site) {
	if (_processResults) {
	    logFunctionalTest(testSuiteName, site, results[site] == 1, label);
	    TESTSET().judgeAndLog_FunctionalTest(results[site] == 1);
	}
}

void FunctionalTest::SMC_backgroundProcessing() {
	for (int i = 0; i < activeSites.size(); i++) {
		int site = activeSites[i];
		processFunc(site);
		if (_processResults) {
			SMC_TEST(site, "", testSuiteName, LIMIT(TM::GE, 1, TM::LE, 1), results[activeSites[i]]);
		}
	}
}

}
}
