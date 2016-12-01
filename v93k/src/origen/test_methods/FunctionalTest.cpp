#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
#include "rdi.hpp"
using namespace std;
#include "origen/test_method/functional_test.hpp"

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
//    	addLimit("testlimit");
    }

    /**
     *This test is invoked per site.
     */
    virtual void run() {
        origen.execute();

//    	string label = Primary.getLabel();
//    	string testSuiteName;
//    	GET_TESTSUITE_NAME(testSuiteName);
//
//    	RDI_INIT();
//    	ON_FIRST_INVOCATION_BEGIN();
//        RDI_BEGIN();
//    	rdi.func(testSuiteName).label(label).execute();
//
////    	cout << testSuiteName << endl;
////    	cout << label << endl;
//    	RDI_END();
//    	cout << "-----------------" << endl;
//        ON_FIRST_INVOCATION_END();
//
//        int site = CURRENT_SITE_NUMBER();
//        int res = rdi.id(testSuiteName).getPassFail();
//       cout << CURRENT_SITE_NUMBER() << ":" << res << endl;
//        Origen::logFunctionalTest(testSuiteName, site, res == 1, label);
//    //    TESTSET().judgeAndLog_FunctionalTest(CURRENT_SITE_NUMBER()  !=  2);

/*
    	string name;
    	GET_TESTSUITE_NAME(name);
    	RDI_INIT();
        ON_FIRST_INVOCATION_BEGIN();
    		FUNCTIONAL_TEST();
//			RDI_BEGIN();
//				rdi.func(name).label(Primary.getLabel()).execute();
//			RDI_END();
        ON_FIRST_INVOCATION_END();

    	cout << "Result is " << GET_FUNCTIONAL_RESULT("@") << endl;
//    	cout << "Result is " << rdi.id(name).getPassFail() << endl;
//    	TESTSET().cont(true).judgeAndLog_FunctionalTest();
//    	TESTSET().cont(true).judgeAndLog_ParametricTest("@","testlimit",GET_FUNCTIONAL_RESULT("@"));
*/
    }

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
