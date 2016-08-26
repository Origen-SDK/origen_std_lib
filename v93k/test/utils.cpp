#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "origen.hpp"

TEST_CASE("Version comparisons work") {
	origen::utils::version v("1.2.3");
    //REQUIRE( Factorial(0) == 1 );
    //REQUIRE( Factorial(1) == 1 );
    //REQUIRE( Factorial(2) == 2 );
    //REQUIRE( Factorial(3) == 6 );
    //REQUIRE( Factorial(10) == 3628800 );
}
