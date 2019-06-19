#include "testmethod.hpp"

// for test method API interfaces
#include "mapi.hpp"
using namespace std;
#include "origen.hpp"

/**
 * Test method class.
 *
 * For each testsuite using this test method, one object of this
 * class is created.
 */
class OrigenRecordBin : public testmethod::TestMethod {
 protected:
  int mBin;
  int mSoftbin;

 protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void initialize() {
    // Add your initialization code here
    // Note: Test Method API should not be used in this method!
    addParameter("bin", "int", &mBin, testmethod::TM_PARAMETER_INPUT);
    addParameter("softbin", "int", &mSoftbin, testmethod::TM_PARAMETER_INPUT);
  }

  /**
   *This test is invoked per site.
   */
  virtual void run() {
    Origen::site().bin(mBin);
    Origen::site().softbin(mSoftbin);
  }
};
REGISTER_TESTMETHOD("RecordBin", OrigenRecordBin);
