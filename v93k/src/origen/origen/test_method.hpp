#ifndef ORIGEN_TEST_METHOD_INCLUDED
#define ORIGEN_TEST_METHOD_INCLUDED

#include "mapi.hpp"
#include "rdi.hpp"
#undef RDI_INIT // The RDI_INIT macro doesn't work when called natively from Origen::TestMethod functions, need to hack it
#define RDI_INIT() SMART_RDI::RDI_RESOURCE::getInstance()->Lego_initialize("");

#define origen getThis()

using namespace std;

namespace Origen {
namespace TestMethod {

}
}

#endif
