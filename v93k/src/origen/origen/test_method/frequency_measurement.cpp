#include "frequency_measurement.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {


FrequencyMeasurement::FrequencyMeasurement() {
    samples(2000);
    processResults(1);
}
FrequencyMeasurement::~FrequencyMeasurement() { }

/// By default the limits are assumed to be expressed as a frequency, set this to 1 if they refer to a target period
FrequencyMeasurement & FrequencyMeasurement::periodBased(int v) { _periodBased = v; return *this; }
/// REQUIRED: The name of the pin being measured
FrequencyMeasurement & FrequencyMeasurement::pin(string v) { _pin = v; return *this; }
/// The number of samples captured by the pattern, the default is 2000
FrequencyMeasurement & FrequencyMeasurement::samples(int v) { _samples = v; return *this; }
/// REQUIRED: Supply the period of the captured vectors in nanoseconds
FrequencyMeasurement & FrequencyMeasurement::periodInNs(int v) { _periodInNs = v; return *this; }
/// When set to 0 the results of the test will not be judged or logged
FrequencyMeasurement & FrequencyMeasurement::processResults(int v) { _processResults = v; return *this; }

// All test methods must implement these functions
FrequencyMeasurement & FrequencyMeasurement::getThis() { return *this; }

void FrequencyMeasurement::execute() {

    int site, physicalSites;
    ARRAY_I sites;

    ON_FIRST_INVOCATION_BEGIN();

    enableHiddenUpload();
    GET_ACTIVE_SITES(activeSites);
    physicalSites = GET_CONFIGURED_SITES(sites);
    funcResults.resize(physicalSites + 1);
    GET_TESTSUITE_NAME(testSuiteName);
    label = Primary.getLabel();
    pin(extractPinsFromGroup(_pin));

    callPreTestFunc();

    RDI_BEGIN();

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
        funcResults[site] = rdi.getBurstPassFail();
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
		logFunctionalTest(testSuiteName, site, funcResults[site] == 1, label);
		TESTSET().testnumber(testnumber()).judgeAndLog_FunctionalTest(funcResults[site] == 1);

		logParametricTest(testSuiteName, site, filterResult(result), limits(), _pin);
		TESTSET().testnumber(testnumber() + 1).judgeAndLog_ParametricTest(_pin, testSuiteName, limits(), filterResult(result));
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
