#include "origen/test_method/dc_measurement.hpp"

/**
 * Test method class.
 *
 * For each testsuite using this test method, one object of this
 * class is created.
 */
class DCMeasurement: public Origen::TestMethod::DCMeasurement {
protected:
  int  mApplyShutdown;
  string  mShutdownPattern;
  string  mMeasure;
  double  mSettlingTime;
  string  mPin;
  string  mPort;
  double  mForceValue;
  double  mIRange;
  int mCheckShutdown;
  int mBadc;

protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void init()
  {
    //Add your initialization code here
    //Note: Test Method API should not be used in this method!
    addParameter("applyShutdown",
                 "int",
                 &mApplyShutdown,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("1")
      .setComment("Apply a shutdown pattern after taking the measurement");
    addParameter("shutdownPattern",
                 "string",
                 &mShutdownPattern,
                 testmethod::TM_PARAMETER_INPUT)
      .setComment("The name of the shutdown pattern, by default it will be the main pattern name appended with '_part1'");
    addParameter("checkShutdown",
                 "int",
                 &mCheckShutdown,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("1")
      .setComment("Check that the shutdown pattern passed (and fail if not)");
    addParameter("measure",
                 "string",
                 &mMeasure,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("VOLT")
      .setComment("Measure 'CURR' or 'VOLT' (the default)");
    addParameter("settlingTime",
                 "double",
                 &mSettlingTime,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("0")
      .setComment("Time to wait between running the pattern and taking the measurement, expressed in ms");
    addParameter("pin",
                 "PinString",
                 &mPin,
                 testmethod::TM_PARAMETER_INPUT)
      .setComment("Pin to be measured");
    addParameter("port",
                 "string",
                 &mPort,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("")
      .setComment("Port the pattern is to be executed on");
    addParameter("forceValue",
                 "double",
                 &mForceValue,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("0")
      .setComment("Value to be forced on the pin during the measurement");
    addParameter("iRange",
                 "double",
                 &mIRange,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("0")
      .setComment("The current range to apply to the measurement (in A), will be calculated if not supplied (set to 0)");
    addParameter("badc",
                 "int",
                 &mBadc,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("0")
      .setComment("Perform the measurement using the Board ADC (rather than the PPMU)");
  }

  /**
   *This test is invoked per site.
   */
  virtual void body()
  {
    origen.applyShutdown(mApplyShutdown)
          .forceValue(mForceValue)
          .shutdownPattern(mShutdownPattern)
          .checkShutdown(mCheckShutdown)
          .measure(mMeasure)
          .settlingTime(mSettlingTime)
          .pin(mPin)
          .port(mPort)
          .iRange(mIRange)
          .badc(mBadc)
          .execute();
  }
};
REGISTER_TESTMETHOD("DCMeasurement", DCMeasurement);
