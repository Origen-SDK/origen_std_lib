#include "test_helper.hpp"
#include "origen/utils/version.hpp"

Origen::Utils::Version v("1.2.3");

TEST_CASE("Version comparisons work") {
	REQUIRE( v.major() == 1);
	REQUIRE( v.minor() == 2);
	REQUIRE( v.tiny() == 3);
	REQUIRE( v == "1.2.3" );
	REQUIRE( v != "2.2.3" );
	REQUIRE( v < "2.2.3" );
	REQUIRE( v < "1.8.3" );
	REQUIRE( v < "1.2.4" );
	REQUIRE( v <= "1.2.4" );
	REQUIRE( v <= "1.2.3" );
	REQUIRE( v > "1.1.3" );
	REQUIRE( v >= "1.1.3" );
	REQUIRE( v >= "1.2.3" );
}

TEST_CASE("Origen::version() is alive") {
	REQUIRE( Origen::version() > "0.0.0" );
	REQUIRE( Origen::version().major() == 0 );
}
