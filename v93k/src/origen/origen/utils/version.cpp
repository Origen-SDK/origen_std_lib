#include "version.hpp"
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

int Version::major() {
    return _major;
}
int Version::minor() {
    return _minor;
}
int Version::tiny() {
    return _tiny;
}

string Version::str() {
    ostringstream str;
    str << _major;
    str << ".";
    str << _minor;
    str << ".";
    str << _tiny;
    return str.str();
}

bool Version::operator==(string rhs) {
    vector<int> v = parse(rhs);
    return _major == v[0] && _minor == v[1] && _tiny == v[2];
}

bool Version::operator!=(string rhs) {
    vector<int> v = parse(rhs);
    return _major != v[0] || _minor != v[1] || _tiny != v[2];
}

bool Version::operator<(string rhs) {
    vector<int> v = parse(rhs);
    return _major < v[0] || (_major == v[0] && _minor < v[1])
           || (_major == v[0] && _minor == v[1] && _tiny < v[2]);
}

bool Version::operator<=(string rhs) {
    return operator==(rhs) || operator<(rhs);
}

bool Version::operator>(string rhs) {
    vector<int> v = parse(rhs);
    return _major > v[0] || (_major == v[0] && _minor > v[1])
           || (_major == v[0] && _minor == v[1] && _tiny > v[2]);
}

bool Version::operator>=(string rhs) {
    return operator==(rhs) || operator>(rhs);
}

vector<int> Version::parse(string ver) {
    vector<int> v(3);

    istringstream parser(ver);
    parser >> v[0];
    for (int i = 1; i < 3; i++) {
        parser.get(); //Skip dot
        parser >> v[i];
    }
    return v;
}

}
}
