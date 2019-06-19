#include "origen/test_method/frequency_measurement.hpp"

class OrigenFrequencyMeasurement
    : public Origen::TestMethod::FrequencyMeasurement {
 protected:
  int mPeriodBased;
  string mPin;
  string mPort;
  int mSamples;
  int mPeriodInNs;

 protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void init() {
    addParameter("periodBased", "int", &mPeriodBased,
                 testmethod::TM_PARAMETER_INPUT)
        .setDefault("0")
        .setComment(
            "Set to 1 if the limits are expressed as a period instead of a "
            "frequency (the default)");
    addParameter("pin", "PinString", &mPin, testmethod::TM_PARAMETER_INPUT)
        .setComment("Pin to be measured");
    addParameter("port", "string", &mPort, testmethod::TM_PARAMETER_INPUT)
        .setDefault("")
        .setComment("Port the pattern is to be executed on");
    addParameter("samples", "int", &mSamples, testmethod::TM_PARAMETER_INPUT)
        .setDefault("2000")
        .setComment(
            "The number of captures made by the pattern, defaults to 2000");
    addParameter("periodInNs", "int", &mPeriodInNs,
                 testmethod::TM_PARAMETER_INPUT)
        .setComment("The period of the capture vectors");
  }

  /**
   *This test is invoked per site.
   */
  virtual void body() {
    origen.periodBased(mPeriodBased)
        .pin(mPin)
        .port(mPort)
        .samples(mSamples)
        .periodInNs(mPeriodInNs)
        .execute();
  }
};
REGISTER_TESTMETHOD("FrequencyMeasurement", OrigenFrequencyMeasurement);
