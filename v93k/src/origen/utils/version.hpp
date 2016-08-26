#ifndef ORIGEN_UTILS_VERSION_INCLUDED
#define ORIGEN_UTILS_VERSION_INCLUDED

#include <string>
using namespace std;

namespace origen {
	namespace utils {

		class Version {

			public:

				Version(std::string& version);
				bool operator<(Version rhs);
	   	};

	}
}

#endif
