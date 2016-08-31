#ifndef ORIGEN_TEST_METHOD_BASE_INCLUDED
#define ORIGEN_TEST_METHOD_BASE_INCLUDED

#include "../test_method.hpp"
#include "origen.hpp"

namespace Origen {
namespace TestMethod {

class Base {
public:
    virtual ~Base() { };

protected:
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
};
}
}

#endif /* BASE_HPP_ */
