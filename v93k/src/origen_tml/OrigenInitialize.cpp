#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
#include "origen.hpp"

/**
 * Test method class.
 *
 * For each testsuite using this test method, one object of this
 * class is created.
 */
class OrigenInitialize: public testmethod::TestMethod {

protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void initialize()
  {
    //Add your initialization code here
    //Note: Test Method API should not be used in this method!
  }

  /**
   *This test is invoked per site.
   */
  virtual void run()
  {
      INT num_of_sites;
      ARRAY_I sites;

      ON_FIRST_INVOCATION_BEGIN();

          num_of_sites = GET_CONFIGURED_SITES(sites);

          // Leave an empty space at 0, easier then to fetch by site number (which start at 1)
          Origen::Sites.resize(num_of_sites + 1, Origen::Site(0));

      ON_FIRST_INVOCATION_END();

      // Instantiate a new site object and save it in the global site array
      Origen::Site site(CURRENT_SITE_NUMBER());

      Origen::Sites[CURRENT_SITE_NUMBER()] = site;

      cout << "LotID:" << Origen::site().lotid() << endl;
      cout << "X:" << Origen::site().x() << endl;
      cout << "Y:" << Origen::site().y() << endl;
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
REGISTER_TESTMETHOD("OrigenInitialize", OrigenInitialize);
