#ifndef ORIGEN_SITE_HPP_
#define ORIGEN_SITE_HPP_

#include <string>
using namespace std;

namespace Origen {

class Site {
    string _lotid;
    bool lotidSet;
    uint8_t _wafer;
    bool waferSet;
    int16_t _x;
    bool xSet;
    int16_t _y;
    bool ySet;
    int _number;


public:
    Site(int);
    virtual ~Site();
    string lotid();
    uint64_t lotidInt();
    void lotid(string);
    uint8_t wafer();
    void wafer(int);
    int16_t x();
    void x(int);
    int16_t y();
    void y(int);
    /// Returns the site number associated with the given site object
    int number() { return _number; }
};


} /* namespace Origen */
#endif
