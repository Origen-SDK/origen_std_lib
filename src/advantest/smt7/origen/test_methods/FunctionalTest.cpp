#include "origen/test_method/functional_test.hpp"


class OrigenFunctionalTest: public Origen::TestMethod::FunctionalTest {
protected:
  string mPort;

protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void init()
  {
    // NOTE: The capture arguments are not listed here since it makes no sense to use them without
    //       corresponding functions to handle the captured data
    
    addParameter("port",
                 "string",
                 &mPort,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("")
      .setComment("Port the pattern is to be executed on");

  }


  virtual void body()
  {
    origen.port(mPort).execute();
  }

};
REGISTER_TESTMETHOD("FunctionalTest", OrigenFunctionalTest);
