#ifndef ORIGEN_UTILS_VERSION_INCLUDED
#define ORIGEN_UTILS_VERSION_INCLUDED

namespace origen {
	namespace utils {
		class Version {

		   	Version(std::string version&);
		   	bool operator<(Version rhs);
	   	};
	};
};

#endif
