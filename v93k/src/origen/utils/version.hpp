#ifndef ORIGEN_UTILS_VERSION_INCLUDED
#define ORIGEN_UTILS_VERSION_INCLUDED

#include <string>
using namespace std;

namespace origen {
	namespace utils {

		class version {

			public:

				version(std::string ver);
				bool operator<(version rhs);
	   	};

	}
}

#endif
