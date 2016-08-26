#ifndef ORIGEN_UTILS_VERSION_INCLUDED
#define ORIGEN_UTILS_VERSION_INCLUDED

// GCC adds these macros by default, which clobbers our functions
// of the same name
#ifdef major
#undef major
#endif
#ifdef minor
#undef minor
#endif

#include <string>
#include <vector>
using namespace std;

namespace Origen {
	namespace Utils {

		// The version class provides an easy way to create conditions based
		// on the version of a library.
		//
		// An instance of the class is returned by Origen.version and which 
		// reflects the current version of the Origen library.
		//
		//   if (Origen.version < "1.1.0") {
		//     // Do something
		//   }
		//
		//   if (Origen.version.major() == 2) {
		//     // Do something
		//   }
		class Version {

			private:
				vector<int> parse(string ver);
				int _major;
				int _minor;
				int _tiny;

			public:
				int major();
				int minor();
				int tiny();
				Version(string ver);
				bool operator==(string rhs);
				bool operator!=(string rhs);
				bool operator<(string rhs);
				bool operator<=(string rhs);
				bool operator>(string rhs);
				bool operator>=(string rhs);
	   	};

	}
}

#endif
