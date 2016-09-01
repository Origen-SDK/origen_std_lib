#include "test_helper.hpp"
#include "origen/test_method/base.hpp"

using namespace std;

class MyTestMethod: public Origen::TestMethod::Base {
	public:
		int c1;

        MyTestMethod() {
			c1 = 0;
		}

		void run() {
			preTestFunc();
			postTestFunc();
		}

    protected: 
		bool preTestFunc() {
			c1 = 1;
		}
};

TEST_CASE("a defined callback gets called") {
	MyTestMethod tm;
	REQUIRE( tm.c1 == 0 );
	tm.run();
	REQUIRE( tm.c1 == 1 );
}
