#include "../origen.hpp"
#include "test_method.hpp"
#include <typeinfo>


using namespace std;

namespace Origen {

	TestMethod::TestMethod() {
		Origen::env()._registerTestMethod(this);
	}

	string TestMethod::className() {
		return typeid(*this).name();
	}

}
