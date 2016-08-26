#include "origen/utils/version.hpp"
using namespace std;

#include <sstream>

namespace Origen {
	namespace Utils {

		Version::Version(string ver) {
			vector<int> v = parse(ver);
			_major = v[0];
			_minor = v[1];
			_tiny = v[2];
		}

		Version::Version() {
			_major = 1;
			_minor = 2;
			_tiny = 3;
		}

		int Version::major() {
			return _major;
		}
		int Version::minor() {
			return _minor;
		}
		int Version::tiny() {
			return _tiny;
		}

		vector<int> Version::parse(string ver) {
			vector<int> v(3);

			istringstream parser(ver);
			parser >> v[0];
			for(int i = 1; i < 3; i++)
			{
				parser.get(); //Skip dot
				parser >> v[i];
			}
			return v;
		}

	}
}
