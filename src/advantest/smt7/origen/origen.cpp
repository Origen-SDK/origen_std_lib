/** @file */ 
#include "origen.hpp"

namespace Origen {

Utils::Version version() {
    Utils::Version v(ORIGEN_VERSION);
    return v;
}

vector<Site> Sites;


Site& site() {
    return Sites[CURRENT_SITE_NUMBER()];
}

Site& site(int site) {
    return Sites[site];
}

bool invertFunctionalResults = false;

}
