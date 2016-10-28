#include "dc_measurement.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {


// Defaults
DCMeasurement::DCMeasurement() {
    applyShutdown(1);
    checkShutdown(1);
    measure("VOLT");
    settlingTime(0);
    forceValue(0);
    iRange(0);
    processResults(1);
    badc(0);
}

DCMeasurement::~DCMeasurement() { }

DCMeasurement & DCMeasurement::applyShutdown(int v) { _applyShutdown = v; return *this; }
DCMeasurement & DCMeasurement::shutdownPattern(string v) { _shutdownPattern = v; return *this; }
DCMeasurement & DCMeasurement::checkShutdown(int v) { _checkShutdown = v; return *this; }
DCMeasurement & DCMeasurement::measure(string v) { _measure = v; return *this; }
DCMeasurement & DCMeasurement::settlingTime(double v) { _settlingTime = v; return *this; }
DCMeasurement & DCMeasurement::pin(string v) { _pin = v; return *this; }
DCMeasurement & DCMeasurement::forceValue(double v) { _forceValue = v; return *this; }
DCMeasurement & DCMeasurement::iRange(double v) { _iRange = v; return *this; }
DCMeasurement & DCMeasurement::processResults(int v) { _processResults = v; return *this; }
DCMeasurement & DCMeasurement::badc(int v) { _badc = v; return *this; }

// All test methods must implement this function
DCMeasurement & DCMeasurement::getThis() { return *this; }

void DCMeasurement::execute() {

    int site, physicalSites;
    ARRAY_I sites;

    RDI_INIT();

    ON_FIRST_INVOCATION_BEGIN();

        enableHiddenUpload();
        GET_ACTIVE_SITES(activeSites);
        physicalSites = GET_CONFIGURED_SITES(sites);
        results.resize(physicalSites + 1);
        funcResultsPre.resize(physicalSites + 1);
        funcResultsPost.resize(physicalSites + 1);
        GET_TESTSUITE_NAME(testSuiteName);
        label = Primary.getLabel();

        pin(extractPinsFromGroup(_pin));

        if (_applyShutdown) {
            if (_shutdownPattern.empty()) {
                ostringstream pat;
                pat << label << "_part1";
                shutdownPattern(pat.str());
            }
        }

        if (!_iRange && _measure == "CURR") {
        	double l = loLimit();
        	double h = hiLimit();
        	if (l == 0 && h == 0) {
                cout << "ERROR: If your current measurement does not have a limit, you must supply the current range" << endl;
                ERROR_EXIT(TM::ABORT_FLOW);
        	}
        	if (abs(l) > abs(h))
        		_iRange = abs(l);
        	else
        	    _iRange = abs(h);
        }


        callPreTestFunc();

        RDI_BEGIN();

        rdi.func(testSuiteName + "f1").label(label).execute();

        callHoldStateFunc();

		if(_measure == "VOLT") {

			if (_badc) {
          		    rdi.dc(testSuiteName)
			       .pin(_pin, TA::BADC)
			       .measWait(_settlingTime)
			       .vMeas()
			       .execute();

			} else {
			    SMART_RDI::dcBase & prdi = rdi.dc(testSuiteName)
                                        .pin(_pin)
                                        .iForce(_forceValue)
                                        .measWait(_settlingTime)
                                        .relay(TA::ppmuRly_onPPMU_offACDC,TA::ppmuRly_onAC_offDCPPMU)
                                        .vMeas();
			    filterRDI(prdi);
			    prdi.execute();

			}



		} else {

			SMART_RDI::dcBase & prdi = rdi.dc(testSuiteName)
										  .pin(_pin)
										  .vForce(_forceValue)
										  .relay(TA::ppmuRly_onPPMU_offACDC,TA::ppmuRly_onAC_offDCPPMU)
										  .measWait(_settlingTime)
										  .iRange(_iRange)
										  .iMeas();
			filterRDI(prdi);
			prdi.execute();
		}

		if (_applyShutdown) rdi.func(testSuiteName + "f2").label(_shutdownPattern).execute();

        RDI_END();

        FOR_EACH_SITE_BEGIN();
            site = CURRENT_SITE_NUMBER();
            funcResultsPre[site] = rdi.id(testSuiteName + "f1").getPassFail();
            if (_applyShutdown) funcResultsPost[site] = rdi.id(testSuiteName + "f2").getPassFail();
            // TODO: This retrieval needs to move to the SMC func in the async case
            if (offline()) {
            	if (!loLimit() && !hiLimit()) {
            		results[site] = 0;
            	} else if(loLimit() && hiLimit()) {
            		results[site] = ((hiLimit() - loLimit()) / 2) + loLimit();
            	} else if (loLimit()) {
            		results[site] = loLimit();
            	} else {
            		results[site] = hiLimit();
            	}

            } else {
            	results[site] = rdi.id(testSuiteName).getValue();
            }

        FOR_EACH_SITE_END();

    ON_FIRST_INVOCATION_END();

    callPostTestFunc(this);
}

void DCMeasurement::serialProcessing(int site) {
	if (_processResults) {
	    logFunctionalTest(testSuiteName, site, funcResultsPre[site] == 1, label);
		TESTSET().cont(true).judgeAndLog_FunctionalTest(funcResultsPre[site] == 1);

		logParametricTest(testSuiteName, site, filterResult(results[site]), limits(), _pin);
		TESTSET().judgeAndLog_ParametricTest(_pin, testSuiteName, limits(), filterResult(results[site]));

        if (_applyShutdown && _checkShutdown) {
            logFunctionalTest(testSuiteName, site, funcResultsPost[site] == 1, _shutdownPattern);
            TESTSET().cont(true).judgeAndLog_FunctionalTest(funcResultsPost[site] == 1);
        }
	}
}

void DCMeasurement::SMC_backgroundProcessing() {
	for (int i = 0; i < activeSites.size(); i++) {
		int site = activeSites[i];
		processFunc(site);
		if (_processResults) {
			SMC_TEST(site, "", testSuiteName, LIMIT(TM::GE, 1, TM::LE, 1), funcResultsPre[site]);
			if (_applyShutdown && _checkShutdown) SMC_TEST(site, "", testSuiteName, LIMIT(TM::GE, 1, TM::LE, 1), funcResultsPost[site]);
			SMC_TEST(site, _pin, testSuiteName, limits(), filterResult(results[site]));
		}
    }
}

}
}
