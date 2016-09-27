#ifndef ORIGEN_TEST_METHOD_BASE_INCLUDED
#define ORIGEN_TEST_METHOD_BASE_INCLUDED

#include "../test_method.hpp"
#include "origen.hpp"

namespace Origen {
namespace TestMethod {

class Base {
    bool _async;
    bool _syncup;

public:
    virtual ~Base() {
    }
    Base() {
        async(false);
        syncup(false);
    }

    Base & async(bool v) { _async = v; return *this; }
    Base & syncup(bool v) { _syncup = v; return *this; }

protected:
    // Returns 1 when running in offline mode
    int offline() {
    	int flag;
        GET_SYSTEM_FLAG("offline", &flag);
        return flag;
    }

    // Returns an object containing the test limits, this can be passed to SMT APIs that take a LIMITS
    // object argument. To actually get the limit values use loLimit() and hiLimit().
    LIMIT limits() {
        return GET_LIMIT_OBJECT("Functional");
    }

    // Returns the high limit value in whole units, i.e. A or V
    double hiLimit() {
        double lim = 0.0;
        double * plim = &lim;

        limits().getHigh(plim);
        return lim;
    }

    // Returns the low limit value in whole units, i.e. A or V
    double loLimit() {
        double lim = 0.0;
        double * plim = &lim;

        limits().getLow(plim);
        return lim;
    }

    // Called immediately before the first RDI operation is executed
    virtual void preTestFunc() {
    }
    virtual void preTestFunc(int site) {
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
    virtual void holdStateFunc() {
    }
    virtual void holdStateFunc(int site) {
    }

    // Called after the last RDI operation has executed and all results have been
    // fetched
    virtual void postTestFunc() {
    }
    virtual void postTestFunc(int site) {
    }

    // Called immediately before the final result processing. If the test is configured for async
    // processing then this will be called later in the background. Contrast this with the
    // postTestFunc which will be called before the main test body completes.
    virtual void processFunc(int site) {
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

    void callPreTestFunc() {
    	if (syncup()) {
			synchronize();
		}
    	preTestFunc();
    	FOR_EACH_SITE_BEGIN();
    	preTestFunc(CURRENT_SITE_NUMBER());
    	FOR_EACH_SITE_END();
    }

    void callHoldStateFunc() {
    	holdStateFunc();
    	FOR_EACH_SITE_BEGIN();
    	holdStateFunc(CURRENT_SITE_NUMBER());
    	FOR_EACH_SITE_END();
    }

    template <class T>
    void callPostTestFunc(T* obj) {
    	ON_FIRST_INVOCATION_BEGIN();
    	postTestFunc();
    	FOR_EACH_SITE_BEGIN();
    	postTestFunc(CURRENT_SITE_NUMBER());
    	FOR_EACH_SITE_END();
    	ON_FIRST_INVOCATION_END();

        if (async()) {
        	SMC_ARM_internal(obj);
        } else {
        	processFunc(CURRENT_SITE_NUMBER());
        	this->serialProcessing(CURRENT_SITE_NUMBER());
		}
    }
};
}
}

#endif /* BASE_HPP_ */
