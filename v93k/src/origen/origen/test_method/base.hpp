#ifndef ORIGEN_TEST_METHOD_BASE_INCLUDED
#define ORIGEN_TEST_METHOD_BASE_INCLUDED

#include "../test_method.hpp"
#include "../../origen.hpp"
#include "testmethod.hpp"
#include "mapi.hpp"
using namespace std;

namespace Origen {
namespace TestMethod {


class Base: public testmethod::TestMethod {
    bool _async;
    bool _syncup;

public:
    Base();
    virtual ~Base();

    Base & async(bool v) { _async = v; return *this; }
    Base & syncup(bool v) { _syncup = v; return *this; }

    void execute() {
    	_execute();
    }

    virtual void SMC_backgroundProcessing() {};

protected:

    int numberOfPhysicalSites;
    ARRAY_I activeSites;
    string suiteName;
    bool bFirstRun;

    string _testName;
    int    _forcePass;
    string _onPassFlag;
    string _onFailFlag;

    int offline();
    void initialize();
    void run();
    int testNumber();
    int testNumber(string);
    TMLimits::LimitInfo testLimits();
    TMLimits::LimitInfo testLimits(string);
    void datalog(double);
    void datalog(string, double);
    void judgeAndDatalog(double);
    void judgeAndDatalog(string, double);
    bool preJudge(double);
    bool preJudge(string, double);
    string testName();
    int invertFunctionalResultIfRequired(int);
    bool isWithinLimits(double, LIMIT);
    LIMIT toNALimit(LIMIT);
    vector<int> suiteFailed;

    virtual void init() {
    }

    /// For internal use, don't override
    virtual void _setup() {
    }

    // Called immediately before the first RDI operation is executed
    virtual void preBody() {
    }
    virtual void preBody(int site) {
    }

    virtual void body() {
    	execute();
    }

    virtual void _execute() {
    }

    // Called before the main RDI operation is executed, giving the chance to add
    // additional settings to it
    virtual void filterRDI(SMART_RDI::dcBase & prdi) {
    }
    virtual void filterRDI(SMART_RDI::DIG_CAP & prdi) {
    }
    virtual void filterRDI(SMART_RDI::FUNC & prdi) {
    }

    // If the test has a hold state, this will be called immediately after the hold
    // state pattern has run
    virtual void holdState() {
    }
    virtual void holdState(int site) {
    }

    // Called after the last RDI operation has executed and all results have been
    // fetched
    virtual void postBody() {
    }
    virtual void postBody(int site) {
    }

    // Called immediately before the final result processing. If the test is configured for async
    // processing then this will be called later in the background. Contrast this with the
    // postTestFunc which will be called before the main test body completes.
    virtual void process(int site) {
    }

    // Called before the main test result is judged, giving a chance to transform it
    virtual double filterResult(double result) {
        return result;
    }

    virtual bool async() {
        return _async;
    }
    virtual bool syncup() {
        return _syncup;
    }

    virtual void serialProcessing(int site) {};

    void enableHiddenUpload() {
        if (async()) {
            rdi.hiddenUpload(TA::ALL);
        } else {
            rdi.hiddenUpload(TA::NO);
        }
    }

    void callPreBody() {
    	if (syncup()) {
			synchronize();
		}
    	preBody();
    	FOR_EACH_SITE_BEGIN();
    	preBody(CURRENT_SITE_NUMBER());
    	FOR_EACH_SITE_END();
    }

    void callHoldState() {
    	holdState();
    	FOR_EACH_SITE_BEGIN();
    	holdState(CURRENT_SITE_NUMBER());
    	FOR_EACH_SITE_END();
    }

    template <class T>
    void callPostBody(T* obj) {
    	ON_FIRST_INVOCATION_BEGIN();
    	postBody();
    	FOR_EACH_SITE_BEGIN();
    	postBody(CURRENT_SITE_NUMBER());
    	FOR_EACH_SITE_END();
    	ON_FIRST_INVOCATION_END();

        if (async()) {
        	SMC_ARM_internal(obj);
        } else {
        	process(CURRENT_SITE_NUMBER());
        	this->serialProcessing(CURRENT_SITE_NUMBER());
		}
    }
};
}
}

#endif /* BASE_HPP_ */
