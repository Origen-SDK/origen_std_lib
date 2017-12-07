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


void initTime(bool reset) {
  if ((!timeSet) || reset) {
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    _month = (timeinfo->tm_mon + 1);
    _day = timeinfo->tm_mday;
    _hour = timeinfo->tm_hour;
    _minute = timeinfo->tm_min;
    _second = timeinfo->tm_sec;
    timeSet = true;
  }
}

/// Get the Month.
int month() {
  initTime();    // set the time if not already
  return _month;
}

/// Get the Day.
int day() {
  initTime();    // set the time if not already
  return _day;
}
/// Get the Hour.
int hour() {
  initTime();    // set the time if not already
  return _hour;
}
/// Get the Minute.
int minute() {
  initTime();    // set the time if not already
  return _minute;
}
/// Get the Second.
int second() {
  initTime();    // set the time if not already
  return _second;
}
   
}  // end namespace Time
}  // end namespace Origen


