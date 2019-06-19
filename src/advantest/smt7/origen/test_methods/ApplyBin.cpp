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
class OrigenApplyBin : public testmethod::TestMethod {
 protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void initialize() {
    // Add your initialization code here
    // Note: Test Method API should not be used in this method!
  }

  /**
   *This test is invoked per site.
   */
  virtual void run() {
    if (Origen::site().bin() || Origen::site().softbin()) {
      stringstream softbin;
      softbin << Origen::site().softbin();
      SET_MULTIBIN(softbin.str(), Origen::site().bin());
      TESTSET().judgeAndLog_FunctionalTest(false);
    }
  }
};
REGISTER_TESTMETHOD("ApplyBin", OrigenApplyBin);
