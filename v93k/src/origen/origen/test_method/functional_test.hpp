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

public:
    FunctionalTest();

    virtual ~FunctionalTest();
    void SMC_backgroundProcessing();
    void execute();

    FunctionalTest & capture(int v);
    FunctionalTest & pin(string v);
    FunctionalTest & bitPerWord(int v);
    FunctionalTest & processResults(int v);
    FunctionalTest & pattern(string v);

protected:
    FunctionalTest & getThis();
    ARRAY_I capturedData();
    ARRAY_I capturedData(int);

    // Internal variables, declared outside the the execute function body since
    // they may be useful in callback functions
    ARRAY_I activeSites;
    string testSuiteName;
    string label;
    vector<int> results;
    string pinName;
};


}
}
#endif
