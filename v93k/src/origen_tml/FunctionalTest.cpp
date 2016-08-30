#include "testmethod.hpp"

#include "mapi.hpp"
#include "rdi.hpp"
#include "origen/test_method.hpp"

using namespace std;

/**
 * Test method class.
 *
 * For each testsuite using this test method, one object of this
 * class is created.
 */
class FunctionalTest: public testmethod::TestMethod,
    public Origen::TestMethod::FunctionalTest {

protected:
    /**
     *Initialize the parameter interface to the testflow.
     *This method is called just once after a testsuite is created.
     */
    virtual void initialize() {
        //Add your initialization code here
        //Note: Test Method API should not be used in this method!
    }

    /**
     *This test is invoked per site.
     */
    virtual void run() {
        executeTest();
        return;
    }

    virtual bool preTestFunc() {
        PUT_DATALOG("Hello from the pre test function!");
        return true;
    }
    ;

    /**
     *This function will be invoked once the specified parameter's value is changed.
     *@param parameterIdentifier
     */
    virtual void postParameterChange(const string& parameterIdentifier) {
        //Add your code
        //Note: Test Method API should not be used in this method!
        return;
    }

    /**
     *This function will be invoked once the Select Test Method Dialog is opened.
     */
    virtual const string getComment() const {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("FunctionalTest", FunctionalTest);
