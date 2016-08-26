#ifndef ORIGEN_UTILS_VERSION_HPP_INCLUDED
#define ORIGEN_UTILS_VERSION_HPP_INCLUDED

namespace origen {
	namespace utils {
		class Version {
		   	Version(std::string const&);
		   	bool operator<(Version const& rhs) const;
	   	};
	};
};

#endif
