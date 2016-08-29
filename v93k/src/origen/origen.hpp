#ifndef ORIGEN_HPP_INCLUDED
#define ORIGEN_HPP_INCLUDED

#define ORIGEN_VERSION "0.0.1"

#ifndef debugger
#define debugger __asm__("int $3");
#endif

#include "origen/utils.hpp"
#include "origen/test_method.hpp"
#include "origen/environment.hpp"
#include "mapi.hpp"

//#define ORIGEN_INIT(...) Origen::_init(this, ##__VA_ARGS__ )

namespace Origen {

	Utils::Version version();
	Environment& env();

}

#endif
