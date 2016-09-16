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

    // Default callback handlers
    virtual bool preTestFunc() {
        return true;
    }
    virtual bool holdStateFunc() {
        return true;
    }
    virtual bool postTestFunc() {
        return true;
    }
    virtual bool preProcessFunc() {
        return true;
    }
    virtual bool processFunc() {
        return true;
    }
    virtual bool postProcessFunc() {
        return true;
    }
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

    template <class T>
    void asyncProcessing(T* obj) {
        if (async()) {
            if (preProcessFunc()) {
                SMC_ARM_internal(obj);
            } else {
                processFunc();
                postProcessFunc();
            }
        }
    }

    void finalProcessing() {
        if (!async()) {
            if (preProcessFunc()) {
                if (processFunc()) {
                    this->serialProcessing(CURRENT_SITE_NUMBER());
                }
            } else {
                processFunc();
            }
            postProcessFunc();
        } else if (syncup()) {
            synchronize();
        }
    }

    void enableHiddenUpload() {
        if (async()) {
            rdi.hiddenUpload(TA::ALL);
        } else {
            rdi.hiddenUpload(TA::NO);
        }
    }
};
}
}

#endif /* BASE_HPP_ */
