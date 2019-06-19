#include "frequency_measurement.hpp"

namespace Origen {
namespace TestMethod {

// Defaults
FrequencyMeasurement::FrequencyMeasurement() {
  samples(2000);
  processResults(1);
  port("");
}
FrequencyMeasurement::~FrequencyMeasurement() {}

/// By default the limits are assumed to be expressed as a frequency, set this
/// to 1 if they refer to a target period
FrequencyMeasurement& FrequencyMeasurement::periodBased(int v) {
  _periodBased = v;
  return *this;
}
/// REQUIRED: The name of the pin being measured
FrequencyMeasurement& FrequencyMeasurement::pin(string v) {
  _pin = v;
  return *this;
}
/// Optionally supply the name of the test port that should be used to execute
/// the pattern
FrequencyMeasurement& FrequencyMeasurement::port(string v) {
  _port = v;
  return *this;
}
/// The number of samples captured by the pattern, the default is 2000
FrequencyMeasurement& FrequencyMeasurement::samples(int v) {
  _samples = v;
  return *this;
}
/// REQUIRED: Supply the period of the captured vectors in nanoseconds
FrequencyMeasurement& FrequencyMeasurement::periodInNs(int v) {
  _periodInNs = v;
  return *this;
}
/// When set to 0 the results of the test will not be judged or logged
FrequencyMeasurement& FrequencyMeasurement::processResults(int v) {
  _processResults = v;
  return *this;
}

// All test methods must implement these functions
FrequencyMeasurement& FrequencyMeasurement::getThis() { return *this; }

void FrequencyMeasurement::_setup() {
  funcResults.resize(numberOfPhysicalSites + 1);
  label = Primary.getLabel();
  pin(extractPinsFromGroup(_pin));
}

void FrequencyMeasurement::_execute() {
  int site;

  ON_FIRST_INVOCATION_BEGIN();

  RDI_BEGIN();

  if (_port.empty()) {
    SMART_RDI::DIG_CAP& prdi =
        rdi.digCap(suiteName).label(label).pin(_pin).bitPerWord(1).samples(
            _samples);

    filterRDI(prdi);
    prdi.execute();

  } else {
    SMART_RDI::DIG_CAP& prdi = rdi.port(_port)
                                   .digCap(suiteName)
                                   .vecVarOnly()
                                   .pin(_pin)
                                   .bitPerWord(1)
                                   .samples(_samples);

    filterRDI(prdi);
    prdi.execute();
    rdi.port(_port).func().burst(label).execute();
  }

  RDI_END();

  FOR_EACH_SITE_BEGIN();
  site = CURRENT_SITE_NUMBER();
  funcResults[site] = rdi.getBurstPassFail();
  FOR_EACH_SITE_END();

  ON_FIRST_INVOCATION_END();
}

void FrequencyMeasurement::serialProcessing(int site) {
  if (_processResults) {
    double result;
    ARRAY_I captureData = rdi.site(site).id(suiteName).getVector();

    if (_periodBased) {
      result = calculatePeriod(captureData, _periodInNs);
    } else {
      result = calculateFrequency(captureData, _periodInNs);
    }

    judgeAndDatalog(testName() + "_FUNC",
                    invertFunctionalResultIfRequired(funcResults[site]));

    judgeAndDatalog(filterResult(result));
  }
}

void FrequencyMeasurement::SMC_backgroundProcessing() {
  double result;

  for (int i = 0; i < activeSites.size(); i++) {
    int site = activeSites[i];
    process(site);
    if (_processResults) {
      ARRAY_I captureData = rdi.site(site).id(suiteName).getVector();
      if (_periodBased) {
        result = calculatePeriod(captureData, _periodInNs);
      } else {
        result = calculateFrequency(captureData, _periodInNs);
      }
      SMC_TEST(site, "", suiteName, LIMIT(TM::GE, 1, TM::LE, 1),
               funcResults[site]);
      SMC_TEST(site, _pin, suiteName, testLimits().TEST_API_LIMIT,
               filterResult(result));
    }
  }
}
}
}
