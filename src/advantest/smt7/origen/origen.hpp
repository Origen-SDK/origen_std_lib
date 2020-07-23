#ifndef ORIGEN_HPP_INCLUDED
#define ORIGEN_HPP_INCLUDED

#define ORIGEN_VERSION "0.13.0"

#ifndef debugger
#define debugger __asm__("int $3");
#endif

#include "origen/helpers.hpp"
#include "origen/site.hpp"
#include "origen/utils.hpp"

namespace Origen {

extern vector<Site> Sites;
Utils::Version version();
Site& site();
Site& site(int site);
extern bool invertFunctionalResults;
}
#endif
