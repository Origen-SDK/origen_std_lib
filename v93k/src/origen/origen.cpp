#include "origen.hpp"

namespace Origen {

	Utils::Version version() {
		Utils::Version v(ORIGEN_VERSION);
		return v;
	}

	Environment& env() {
		return Environment::getInstance();
	}

}
