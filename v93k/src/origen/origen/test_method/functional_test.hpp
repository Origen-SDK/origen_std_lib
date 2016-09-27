#ifndef ORIGEN_TEST_METHOD_FUNCTIONAL_TEST_INCLUDED
#define ORIGEN_TEST_METHOD_FUNCTIONAL_TEST_INCLUDED

#include "base.hpp"
#include "mapi.hpp"
#include "rdi.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {

class FunctionalTest: public Base  {
    void serialProcessing(int site);

    string _pin;
    int _capture;
    int _bitPerWord;
    int _processResults;

public:
    // Defaults
    FunctionalTest() {
    	pin("");
    	capture(0);
        processResults(1);
        bitPerWord(1);
    }

    virtual ~FunctionalTest() { }
    void SMC_backgroundProcessing();
    void execute();

    /// The functional test method can optionally capture data by supplying the number of vectors to capture
    FunctionalTest & capture(int v) { _capture = v; return *this; }
    /// If data capture is requested, supply the pin to capture data from
    FunctionalTest & pin(string v) { _pin = v; return *this; }
    /// Serial capture data will be grouped into words, specify how many bits per word in the serial stream (default 1)
    FunctionalTest & bitPerWord(int v) { _bitPerWord = v; return *this; }
    /// When set to 0 the results of the test will not be judged or logged
    FunctionalTest & processResults(int v) { _processResults = v; return *this; }

protected:
    // All test methods must implement these functions
    FunctionalTest & getThis() { return *this; }

    // Internal variables, declared outside the the execute function body since
    // they may be useful in callback functions
    ARRAY_I activeSites;
    string testSuiteName;
    string label;
    vector<int> results;
};

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

    RDI_BEGIN();

    callPreTestFunc();

    if (_capture) {
        SMART_RDI::DIG_CAP & prdi = rdi.digCap(testSuiteName)
    								   .label(label)
    								   .pin(extractPinsFromGroup(_pin))
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
        results[site] = rdi.id(testSuiteName).getPassFail();
    FOR_EACH_SITE_END();

    ON_FIRST_INVOCATION_END();

    callPostTestFunc(this);
}

void FunctionalTest::serialProcessing(int site) {
	if (_processResults) {
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
#endif
