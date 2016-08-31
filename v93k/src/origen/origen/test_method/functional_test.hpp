#ifndef ORIGEN_TEST_METHOD_FUNCTIONAL_TEST_INCLUDED
#define ORIGEN_TEST_METHOD_FUNCTIONAL_TEST_INCLUDED

#include "base.hpp"
#include "mapi.hpp"
#include "rdi.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {

class FunctionalTest: public Base  {

public:
    virtual ~FunctionalTest() {
    }
    ;
    void SMC_backgroundProcessing();
    void execute();

protected:
    // All test methods must implement this function
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

    rdi.hiddenUpload(TA::ALL); // Enable Smart Calc
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

    ON_FIRST_INVOCATION_END();

}

//void TestMethod::SMC_backgroundProcessing(const ARRAY_I& sites,
//		const string& label, const string& vsup) {
void FunctionalTest::SMC_backgroundProcessing() {
    if (processFunc()) {
        for (int i = 0; i < activeSites.size(); i++) {
            int site = activeSites[i];
            SMC_TEST(site, "", testSuiteName, LIMIT(TM::GE, 1, TM::LE, 1), results[activeSites[i]]);
        }
    }
    postProcessFunc();
}

}
}
#endif
