#ifndef ORIGEN_TEST_METHOD_FREQUENCY_MEASUREMENT_INCLUDED
#define ORIGEN_TEST_METHOD_FREQUENCY_MEASUREMENT_INCLUDED

#include "base.hpp"
#include "mapi.hpp"
#include "rdi.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {

class FrequencyMeasurement: public Base  {
    void serialProcessing(int site);

    int _periodBased;
    string _pin;
    int _samples;
    int _periodInNs;
    int _processResults;

public:
    FrequencyMeasurement();
    virtual ~FrequencyMeasurement();
    void SMC_backgroundProcessing();
    void execute();

    FrequencyMeasurement & periodBased(int v);
    FrequencyMeasurement & pin(string v);
    FrequencyMeasurement & samples(int v);
    FrequencyMeasurement & periodInNs(int v);
    FrequencyMeasurement & processResults(int v);

protected:
    FrequencyMeasurement & getThis();

    // Internal variables, declared outside the the execute function body since
    // they may be useful in callback functions
    ARRAY_I activeSites;
    string testSuiteName;
    string label;
    vector<int> funcResults;

};

}
}
#endif
