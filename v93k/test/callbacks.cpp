#include "test_helper.hpp"
#include "origen.hpp"
#include <iostream>

using namespace std;

class MyTestMethod: public Origen::TestMethod {
	public:
		int c1;
		int c2;
		void setup() {
			c1 = 0;
			c2 = 0;
		};
		void exec() { run(); };

	protected:
		virtual void run() {
			preExecFunc();
			postExecFunc();
		};

		void preExecFunc() {
			c1 = 1;
		}
};

TEST_CASE("a defined callback gets called") {
	MyTestMethod tm;
	tm.setup();
	REQUIRE( tm.c1 == 0 );
	tm.exec();
	REQUIRE( tm.c1 == 1 );
}
