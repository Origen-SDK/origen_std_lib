#ifndef ORIGEN_TEST_METHOD_INCLUDED
#define ORIGEN_TEST_METHOD_INCLUDED

#include <string>
using namespace std;

namespace Origen {

	class TestMethod {
		protected:
			virtual void preExecFunc() {};
			virtual void postExecFunc() {};
	};
}

#endif
