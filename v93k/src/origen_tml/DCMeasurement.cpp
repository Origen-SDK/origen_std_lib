#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
#include "origen/test_method/dc_measurement.hpp"

/**
 * Test method class.
 *
 * For each testsuite using this test method, one object of this
 * class is created.
 */
class DCMeasurement: public testmethod::TestMethod, public Origen::TestMethod::DCMeasurement {
protected:
  int  mApplyShutdown;
  string  mShutdownPattern;
  string  mMeasure;
  double  mSettlingTime;
  string  mPin;
  double  mForceValue;

protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void initialize()
  {
    //Add your initialization code here
    //Note: Test Method API should not be used in this method!
    addParameter("ApplyShutdown",
                 "int",
                 &mApplyShutdown,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("1")
      .setComment("Apply a shutdown pattern after taking the measurement");
    addParameter("ShutdownPattern",
                 "string",
                 &mShutdownPattern,
                 testmethod::TM_PARAMETER_INPUT)
      .setComment("The name of the shutdown pattern, by default it will be the main pattern name appended with '_part1'");
    addParameter("Measure",
                 "string",
                 &mMeasure,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("VOLT")
      .setComment("Measure 'CURR' or 'VOLT' (the default)");
    addParameter("SettlingTime",
                 "double",
                 &mSettlingTime,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("0")
      .setComment("Time to wait between running the pattern and taking the measurement, expressed in ms");
    addParameter("Pin",
                 "PinString",
                 &mPin,
                 testmethod::TM_PARAMETER_INPUT)
      .setComment("Pin to be measured");
    addParameter("ForceValue",
                 "double",
                 &mForceValue,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("0")
      .setComment("Value to be forced on the pin during the measurement");
    addLimit("Functional");
  }

  /**
   *This test is invoked per site.
   */
  virtual void run()
  {
    origen.forceValue(mForceValue).pin(mPin).execute();
    return;
  }

  /**
   *This function will be invoked once the specified parameter's value is changed.
   *@param parameterIdentifier
   */
  virtual void postParameterChange(const string& parameterIdentifier)
  {
    //Add your code
    //Note: Test Method API should not be used in this method!
    return;
  }

  /**
   *This function will be invoked once the Select Test Method Dialog is opened.
   */
  virtual const string getComment() const 
  {
    string comment = " please add your comment for this method.";
    return comment;
  }
};
REGISTER_TESTMETHOD("DCMeasurement", DCMeasurement);
