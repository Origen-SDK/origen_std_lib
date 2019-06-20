#include "base.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {

Base::Base() { syncup(false); }

Base::~Base() {}

/// Returns 1 when running in offline mode
int Base::offline() {
  int flag;
  GET_SYSTEM_FLAG("offline", &flag);
  return flag;
}

void Base::initialize() {
  addParameter("testName", "string", &_testName,
               testmethod::TM_PARAMETER_INPUT);
  addParameter("forcePass", "int", &_forcePass, testmethod::TM_PARAMETER_INPUT);
  addParameter("onPassFlag", "string", &_onPassFlag,
               testmethod::TM_PARAMETER_INPUT);
  addParameter("onFailFlag", "string", &_onFailFlag,
               testmethod::TM_PARAMETER_INPUT);
  addParameter("async", "int", &_async, testmethod::TM_PARAMETER_INPUT)
      .setDefault(0)
      .setComment(
          "When true, background processing of the results of this test will "
          "be enabled");

  bFirstRun = true;

  init();
}

void Base::run() {
  ARRAY_I sites;

  RDI_INIT();

  ON_FIRST_INVOCATION_BEGIN();

  GET_ACTIVE_SITES(activeSites);
  numberOfPhysicalSites = GET_CONFIGURED_SITES(sites);
  GET_TESTSUITE_NAME(suiteName);
  suiteFailed.resize(numberOfPhysicalSites + 1);

  _setup();

  callPreBody();

  enableHiddenUpload();

  ON_FIRST_INVOCATION_END();

  suiteFailed[CURRENT_SITE_NUMBER()] = 0;

  body();

  callPostBody(this);

  if (suiteFailed[CURRENT_SITE_NUMBER()]) {
    if (_onFailFlag != "") {
      SET_USER_DOUBLE(_onFailFlag, 1);
    }
  } else {
    if (_onPassFlag != "") {
      SET_USER_DOUBLE(_onPassFlag, 1);
    }
  }

  bFirstRun = false;
}

void Base::datalog(double value) {
  TESTSET()
      .testnumber(testNumber())
      .cont(true)
      .TEST("", testName(), noLimits(), value);
}

void Base::datalog(string testName, double value) {
  TESTSET()
      .testnumber(testNumber(testName))
      .cont(true)
      .TEST("", testName, noLimits(), value);
}

void Base::judgeAndDatalog(double value) {
  judgeAndDatalog(value, CURRENT_SITE_NUMBER());
}

void Base::judgeAndDatalog(double value, int site) {
  bool alreadyFailed = suiteFailed[site];

  if (!alreadyFailed) {
    suiteFailed[site] = !preJudge(value);
  }

  TESTSET()
      .testnumber(testNumber())
      .cont(true)
      .judgeAndLog_ParametricTest(
          "", testName(), _forcePass ? toNALimit(testLimits().TEST_API_LIMIT)
                                     : testLimits().TEST_API_LIMIT,
          value);

  // Preserve the first bin assigned within this test suite as the final one
  if ((!alreadyFailed) && (!_forcePass) && (suiteFailed[site])) {
    SET_MULTIBIN(testLimits().BinsNumString, testLimits().BinhNum);
  }
}

void Base::judgeAndDatalog(string testName, double value) {
  judgeAndDatalog(testName, value, CURRENT_SITE_NUMBER());
}

void Base::judgeAndDatalog(string testName, double value, int site) {
  bool alreadyFailed = suiteFailed[site];

  if (!alreadyFailed) {
    suiteFailed[site] = !preJudge(testName, value);
  }

  TESTSET()
      .testnumber(testNumber(testName))
      .cont(true)
      .judgeAndLog_ParametricTest(
          "", testName,
          _forcePass ? toNALimit(testLimits(testName).TEST_API_LIMIT)
                     : testLimits(testName).TEST_API_LIMIT,
          value);

  // Preserve the first bin assigned within this test suite as the final one
  if ((!alreadyFailed) && (!_forcePass) && (suiteFailed[site])) {
    SET_MULTIBIN(testLimits().BinsNumString, testLimits().BinhNum);
  }
}

/// Returns true if the given value will pass the current test, but does not
/// affect the site status if it fails
bool Base::preJudge(double value) {
  return isWithinLimits(value, testLimits().TEST_API_LIMIT);
}

/// Returns true if the given value will pass the given test, but does not
/// affect the site status if it fails
bool Base::preJudge(string testName, double value) {
  return isWithinLimits(value, testLimits(testName).TEST_API_LIMIT);
}

/// Returns true if the given value is within the given limits
bool Base::isWithinLimits(double value, LIMIT limits) {
  bool passed = true;
  double dHigh(0), dLow(0);
  TM::COMPARE cHigh, cLow;

  limits.get(cLow, dLow, cHigh, dHigh);

  if (cLow == TM::GT && value <= dLow) {
    passed = false;
  } else if (cLow == TM::GE && value < dLow) {
    passed = false;
  }

  if (cHigh == TM::LT && value >= dHigh) {
    passed = false;
  } else if (cHigh == TM::LE && value > dHigh) {
    passed = false;
  }
  return passed;
}

/// Converts the given limits object to an equivalent version with the limit
/// types set to N/A
LIMIT Base::toNALimit(LIMIT limits) {
  LIMIT naLimit(limits);
  double dHigh(0), dLow(0);
  limits.getHigh(&dHigh);
  limits.getLow(&dLow);
  naLimit.high(TM::NA, dHigh);
  naLimit.low(TM::NA, dLow);
  return naLimit;
}

/// Returns the base test number
int Base::testNumber() { return testLimits().TestNumber; }

/// Returns the test test number for the given test name
int Base::testNumber(string testName) {
  return testLimits(testName).TestNumber;
}

/// Returns the base test limits
TMLimits::LimitInfo Base::testLimits() {
  // Doesn't seem like this should be required from the documentation, but had
  // some
  // problems with getLimitRef not working properly without it when other code
  // has set
  // a specific key.
  tmLimits.setDefaultLookupKeys();
  return tmLimits.getLimitRef(suiteName, testName());
}

/// Returns the test limits for the given test name
TMLimits::LimitInfo Base::testLimits(string testName) {
  // Doesn't seem like this should be required from the documentation, but had
  // some
  // problems with getLimitRef not working properly without it when other code
  // has set
  // a specific key.
  tmLimits.setDefaultLookupKeys();
  return tmLimits.getLimitRef(suiteName, testName);
}

/// Returns the value of the testName parameter supplied from the test suite, or
/// if not supplied falls back
/// to the test suite name
string Base::testName() {
  if (_testName == "") {
    return suiteName;
  } else {
    return _testName;
  }
}

/// Changes a 0 -> 1 and 1 -> 0 if Origen::invertFunctionalResults has been set
/// to true
int Base::invertFunctionalResultIfRequired(int v) {
  if (Origen::invertFunctionalResults) {
    return v == 1 ? 0 : 1;
  } else {
    return v;
  }
}
}
}
