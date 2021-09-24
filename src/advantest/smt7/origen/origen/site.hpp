#ifndef ORIGEN_SITE_HPP_
#define ORIGEN_SITE_HPP_

#include <string>
#include <inttypes.h>
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
  int _bin;
  int _softbin;
  bool binSet;
  bool softbinSet;

 public:
  Site(int);
  virtual ~Site();
  string lotid();
  uint64_t lotidInt();
  void lotid(string);
  void lotid(uint64_t);
  int wafer();
  void wafer(int);
  int x();
  void x(int);
  int y();
  void y(int);
  int bin();
  void bin(int);
  void bin(int, bool);
  int softbin();
  void softbin(int);
  void softbin(int, bool);

  /// Returns the site number associated with the given site object
  int number() { return _number; }
};

} /* namespace Origen */
#endif
