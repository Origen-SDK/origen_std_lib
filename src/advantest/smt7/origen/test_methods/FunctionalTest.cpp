#include "origen/test_method/functional_test.hpp"


class FunctionalTest: public Origen::TestMethod::FunctionalTest {

};
REGISTER_TESTMETHOD("FunctionalTest", FunctionalTest);
