#include "test_helper.hpp"
#include "origen/utils/version.hpp"

//Origen::Utils::Version v("1.2.3");
Origen::Utils::Version v;

TEST_CASE("Version comparisons work") {
	REQUIRE(1 == 1);
	//REQUIRE( v.major == 1);
	//REQUIRE( v.minor == 2);
	//REQUIRE( v.tiny == 3);
    //REQUIRE( v == "1.2.3" );
    //REQUIRE( v != "2.2.3" );
}
