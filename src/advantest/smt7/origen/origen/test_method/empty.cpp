#include "empty.hpp"

namespace Origen {
namespace TestMethod {

Empty::Empty() {}

Empty::~Empty() {}

// All test methods must implement this function
Empty& Empty::getThis() { return *this; }
}
}
