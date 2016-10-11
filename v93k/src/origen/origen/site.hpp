#ifndef ORIGEN_SITE_HPP_
#define ORIGEN_SITE_HPP_

#include <string>
using namespace std;

namespace Origen {

class Site {
    string _lotid;
    bool lotidSet;
    int _wafer;
    bool waferSet;
    int _x;
    bool xSet;
    int _y;
    bool ySet;
    int _number;


public:
    Site(int);
    virtual ~Site();
    string lotid();
    void lotid(string);
    int wafer();
    void wafer(int);
    int x();
    void x(int);
    int y();
    void y(int);
    /// Returns the site number associated with the given site object
    int number() { return _number; }
};


} /* namespace Origen */
#endif
