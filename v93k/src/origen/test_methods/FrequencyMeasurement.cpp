#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
#include "../origen/test_method/frequency_measurement.hpp"

/**
 * Test method class.
 *
 * For each testsuite using this test method, one object of this
 * class is created.
 */
class FrequencyMeasurement: public testmethod::TestMethod,
                            public Origen::TestMethod::FrequencyMeasurement {

protected:                                 
  int    mPeriodBased;
  string mPin;
  int    mSamples;
  int    mPeriodInNs;

protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void initialize()
  {
    addParameter("periodBased",
                 "int",
                 &mPeriodBased,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("0")
      .setComment("Set to 1 if the limits are expressed as a period instead of a frequency (the default)");
    addParameter("pin",
                 "PinString",
                 &mPin,
                 testmethod::TM_PARAMETER_INPUT)
      .setComment("Pin to be measured");
    addParameter("samples",
                 "int",
                 &mSamples,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("2000")
      .setComment("The number of captures made by the pattern, defaults to 2000");
    addParameter("periodInNs",
                 "int",
                 &mPeriodInNs,
                 testmethod::TM_PARAMETER_INPUT)
      .setComment("The period of the capture vectors");


    //Add your initialization code here
    //Note: Test Method API should not be used in this method!
    addLimit("Functional");
  }

  /**
   *This test is invoked per site.
   */
  virtual void run()
  {
      origen.periodBased(mPeriodBased)
            .pin(mPin)
            .samples(mSamples)
            .periodInNs(mPeriodInNs)
            .execute();
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
REGISTER_TESTMETHOD("FrequencyMeasurement", FrequencyMeasurement);
