#ifndef ORIGEN_HPP_INCLUDED
#define ORIGEN_HPP_INCLUDED

#define ORIGEN_VERSION "0.8.1"

#ifndef debugger
#define debugger __asm__("int $3");
#endif

#include "origen/utils.hpp"
#include "origen/helpers.hpp"
#include "origen/site.hpp"

namespace Origen {

extern vector<Site> Sites;
Utils::Version version();
Site& site();
Site& site(int site);

}
#endif
