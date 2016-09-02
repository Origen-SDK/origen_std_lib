#ifndef ORIGEN_HPP_INCLUDED
#define ORIGEN_HPP_INCLUDED

#define ORIGEN_VERSION "0.2.0"

#ifndef debugger
#define debugger __asm__("int $3");
#endif

#include "origen/utils.hpp"
#include "origen/helpers.hpp"

namespace Origen {

Utils::Version version();

}

#endif
