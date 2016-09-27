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
    // Defaults
    FrequencyMeasurement() {
        samples(2000);
        processResults(1);
    }
    virtual ~FrequencyMeasurement() { }
    void SMC_backgroundProcessing();
    void execute();

    /// By default the limits are assumed to be expressed as a frequency, set this to 1 if they refer to a target period
    FrequencyMeasurement & periodBased(int v) { _periodBased = v; return *this; }
    /// REQUIRED: The name of the pin being measured
    FrequencyMeasurement & pin(string v) { _pin = v; return *this; }
    /// The number of samples captured by the pattern, the default is 2000
    FrequencyMeasurement & samples(int v) { _samples = v; return *this; }
    /// REQUIRED: Supply the period of the captured vectors in nanoseconds
    FrequencyMeasurement & periodInNs(int v) { _periodInNs = v; return *this; }
    /// When set to 0 the results of the test will not be judged or logged
    FrequencyMeasurement & processResults(int v) { _processResults = v; return *this; }

protected:
    // All test methods must implement these functions
    FrequencyMeasurement & getThis() { return *this; }

    // Internal variables, declared outside the the execute function body since
    // they may be useful in callback functions
    ARRAY_I activeSites;
    string testSuiteName;
    string label;
    vector<int> funcResults;
};

void FrequencyMeasurement::execute() {

    int site, physicalSites;
    ARRAY_I sites;

    RDI_INIT();

    ON_FIRST_INVOCATION_BEGIN();

    enableHiddenUpload();
    GET_ACTIVE_SITES(activeSites);
    physicalSites = GET_CONFIGURED_SITES(sites);
    funcResults.resize(physicalSites + 1);
    GET_TESTSUITE_NAME(testSuiteName);
    label = Primary.getLabel();
    pin(extractPinsFromGroup(_pin));

    RDI_BEGIN();

    callPreTestFunc();

    SMART_RDI::DIG_CAP & prdi = rdi.digCap(testSuiteName)
								   .label(label)
								   .pin(_pin)
								   .bitPerWord(1)
								   .samples(_samples);

	filterRDI(prdi);
	prdi.execute();

    RDI_END();

    FOR_EACH_SITE_BEGIN();
        site = CURRENT_SITE_NUMBER();
        funcResults[site] = rdi.id(testSuiteName).getPassFail();
    FOR_EACH_SITE_END();

    ON_FIRST_INVOCATION_END();

    callPostTestFunc(this);
}

void FrequencyMeasurement::serialProcessing(int site) {
	if (_processResults) {
		double result;
		ARRAY_I captureData = rdi.site(site).id(testSuiteName).getVector();

		if (_periodBased) {
			result = calculatePeriod(captureData, _periodInNs);
		} else {
			result = calculateFrequency(captureData, _periodInNs);
		}
		TESTSET().judgeAndLog_FunctionalTest(funcResults[site]);
		TESTSET().judgeAndLog_ParametricTest(_pin, testSuiteName, limits(), filterResult(result));
	}
}

void FrequencyMeasurement::SMC_backgroundProcessing() {
    double result;

	for (int i = 0; i < activeSites.size(); i++) {
		int site = activeSites[i];
		processFunc(site);
		if (_processResults) {
			ARRAY_I captureData = rdi.site(site).id(testSuiteName).getVector();
			if (_periodBased) {
				result = calculatePeriod(captureData, _periodInNs);
			} else {
				result = calculateFrequency(captureData, _periodInNs);
			}
			SMC_TEST(site, "", testSuiteName, LIMIT(TM::GE, 1, TM::LE, 1), funcResults[site]);
			SMC_TEST(site, _pin, testSuiteName, limits(), filterResult(result));
		}
	}
}

}
}
#endif
