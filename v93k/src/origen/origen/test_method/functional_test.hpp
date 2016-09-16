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

    int _processResults;

public:
    // Defaults
    FunctionalTest() {
        processResults(1);
    }

    virtual ~FunctionalTest() { }
    void SMC_backgroundProcessing();
    void execute();

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

    SMART_RDI::FUNC & prdi = rdi.func(testSuiteName + "f1").label(label);

	filterRDI(prdi);
	prdi.execute();

    RDI_END();

    FOR_EACH_SITE_BEGIN();
        site = CURRENT_SITE_NUMBER();
        results[site] = rdi.id(testSuiteName + "f1").getPassFail();
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
	processFunc();

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
