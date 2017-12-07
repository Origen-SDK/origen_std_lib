/** @file */ 
#include "../helpers.hpp"
#include <cerrno>
#include <stdlib.h>
#include "../../origen.hpp"

using namespace std;

namespace Origen {

namespace Time {

static bool timeSet = false;
static int _month;
static int _day;
static int _hour;
static int _minute;
static int _second;

static time_t rawtime;
static struct tm * timeinfo;


void getTime(bool reset) {
  time (&rawtime);
  timeinfo = localtime (&rawtime);

  if ((!timeSet) || reset) {
    _month = (timeinfo->tm_mon + 1);
    _day = timeinfo->tm_mday;
    _hour = timeinfo->tm_hour;
    _minute = timeinfo->tm_min;
    _second = timeinfo->tm_sec;
    timeSet = true;
  }
}

/// Get the Month. If not set will query the time for any
/// subsequent time type calls
int month() {
  if (!timeSet) {
    getTime();
    timeSet = true;
  }
  return _month;
}

/// Get the Day. Requires to run setTime prior to grab the time.
int day() {
  if (!timeSet) {
    getTime();
    timeSet = true;
  }
  return _day;
}
/// Get the Month. Requires to run setTime prior to grab the time.
int hour() {
  if (!timeSet) {
    getTime();
    timeSet = true;
  }
  return _hour;
}
/// Get the Month. Requires to run setTime prior to grab the time.
int minute() {
  if (!timeSet) {
    getTime();
    timeSet = true;
  }
  return _minute;
}
/// Get the Month. Requires to run setTime prior to grab the time.
int second() {
  if (!timeSet) {
    getTime();
    timeSet = true;
  }
  return _second;
}
   
}  // end namespace Time
}  // end namespace Origen


