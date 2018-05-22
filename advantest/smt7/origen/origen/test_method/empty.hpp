#ifndef ORIGEN_TEST_METHOD_FUNCTIONAL_TEST_INCLUDED
#define ORIGEN_TEST_METHOD_FUNCTIONAL_TEST_INCLUDED

#include "base.hpp"
#include "mapi.hpp"
#include "rdi.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {

class Empty: public Base  {

public:
    Empty();
    virtual ~Empty();

protected:
    typedef Empty super;
    Empty & getThis();
};


}
}
#endif
