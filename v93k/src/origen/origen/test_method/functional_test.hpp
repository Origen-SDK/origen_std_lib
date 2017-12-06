#ifndef ORIGEN_TEST_METHOD_FUNCTIONAL_TEST_INCLUDED
#define ORIGEN_TEST_METHOD_FUNCTIONAL_TEST_INCLUDED

#include "base.hpp"
#include "mapi.hpp"
#include "rdi.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {

class FunctionalTest: public Base  {
    void serialProcessing(int site);

    string _pin;
    int _capture;
    int _bitPerWord;
    int _processResults;
    string _pattern;
    string _testNameOverride;

public:
    FunctionalTest();
    virtual ~FunctionalTest();
    void SMC_backgroundProcessing();

    FunctionalTest & capture(int v);
    FunctionalTest & pin(string v);
    FunctionalTest & bitPerWord(int v);
    FunctionalTest & processResults(int v);
    FunctionalTest & pattern(string v);
    FunctionalTest & testName(string v);

    string testName() {
    	return Base::testName();
    }

protected:
    typedef FunctionalTest super;
    FunctionalTest & getThis();
    ARRAY_I capturedData();
    ARRAY_I capturedData(int);
    void _setup();
    void _execute();

    string label;
    vector<int> results;
    string pinName;
};


}
}
#endif
