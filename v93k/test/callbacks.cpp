#include "test_helper.hpp"
#include "origen.hpp"
#include <iostream>

using namespace std;

class MyTestMethod: public Origen::TestMethod {
	public:
		int c1;
		int c2;
		void run();
		void setup();
};


void MyTestMethod::run() {
	return;	
}

void MyTestMethod::setup() {
	c1 = 0;
	c2 = 0;
}

TEST_CASE("TestMethod#className works") {
	MyTestMethod tm;
	REQUIRE( tm.className().find("MyTestMethod") != std::string::npos );
}

TEST_CASE("a defined callback gets called") {
	MyTestMethod tm;
	tm.setup();
	cout << tm.className() << endl;
	REQUIRE( tm.c1 == 0 );
	REQUIRE( tm.c2 == 0 );
	tm.run();
	REQUIRE( tm.c1 == 1 );
	REQUIRE( tm.c2 == 0 );
}
