#ifndef ORIGEN_TEST_METHOD_DC_MEASUREMENT_INCLUDED
#define ORIGEN_TEST_METHOD_DC_MEASUREMENT_INCLUDED

#include "../test_method.hpp"
#include "mapi.hpp"
#include "rdi.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {

class DCMeasurement {

public:
    virtual ~DCMeasurement() {
    }
    ;
    void SMC_backgroundProcessing();

protected:
    // Main APIs to perform a test
    void executeTest();

    // Default callback handlers
    virtual bool preTestFunc() {
        return true;
    }
    ;
    virtual bool postTestFunc() {
        return true;
    }
    ;
    virtual bool preProcessFunc() {
        return true;
    }
    ;
    virtual bool processFunc() {
        return true;
    }
    ;
    virtual bool postProcessFunc() {
        return true;
    }
    ;

    // Internal variables, declared outside the the main execute function body since
    // they may be useful in callback functions
    ARRAY_I activeSites;
    string testSuiteName;
    string label;
    vector<int> results;
};

void DCMeasurement::executeTest() {

    int site, physicalSites;
    ARRAY_I sites;

    RDI_INIT();

    ON_FIRST_INVOCATION_BEGIN();

    label = Primary.getLabel();
    // Enable Smart Calc
    rdi.hiddenUpload(TA::ALL);
    GET_ACTIVE_SITES(activeSites);
    physicalSites = GET_CONFIGURED_SITES(sites);
    results.resize(physicalSites + 1);
    GET_TESTSUITE_NAME(testSuiteName);
    label = Primary.getLabel();

    RDI_BEGIN();

    if (preTestFunc()) {
        rdi.func("f1").label(label).execute();
    }

    RDI_END();

    postTestFunc();

    FOR_EACH_SITE_BEGIN();
    site = CURRENT_SITE_NUMBER();
    results[site] = rdi.id("f1").getPassFail();
    FOR_EACH_SITE_END();

    if (preProcessFunc()) {
        SMC_ARM();
    } else {
        processFunc();
        postProcessFunc();
    }

    ON_FIRST_INVOCATION_END()
    ;

}

//void TestMethod::SMC_backgroundProcessing(const ARRAY_I& sites,
//		const string& label, const string& vsup) {
void DCMeasurement::SMC_backgroundProcessing() {
    if (processFunc()) {
        for (int i = 0; i < activeSites.size(); i++) {
            SMC_TEST("", testSuiteName, LIMIT(TM::GE, 1, TM::LE, 1), results[activeSites[i]]);
        }
    }
    postProcessFunc();
}

}
}
#endif
