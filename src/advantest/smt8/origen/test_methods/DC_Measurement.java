package origen.test_methods;

import java.io.File;

import origen.common.Origen;
import origen.common.OrigenHelpers;
import xoc.dsa.DeviceSetupFactory;
import xoc.dsa.IDeviceSetup;
import xoc.dta.datatypes.MultiSiteBoolean;
import xoc.dta.datatypes.MultiSiteDouble;
import xoc.dta.datatypes.MultiSiteDoubleArray;
import xoc.dta.resultaccess.IDigInOutActionResults;
import xoc.dta.resultaccess.IDigInOutActionResults.IIforceVmeasResults;
import xoc.dta.resultaccess.IDigInOutActionResults.IVforceImeasResults;
import xoc.dta.resultaccess.IMeasurementResult;
import xoc.dta.testdescriptor.IParametricTestDescriptor;

/**
 * An example test method using test descriptor
 */
public class DC_Measurement extends Base {

    public enum MEAS
    {
        CURR, VOLT
    }

    MultiSiteDoubleArray _data;

    boolean _applyShutdown;
    String _shutdownPattern;
    int _checkShutdown;
    MEAS _measure;
    double _settlingTime;
    String _pin;
    double _forceValue;
    double _iRange;
    double _VclampLow;
    double _VclampHigh;
    boolean _highAccuracy;
    double _limitsLow;
    double _limitsHigh;
    int _averages;

    int _badc;
    String _actionName;
    public DC_Measurement origen;

    static OrigenHelpers origenHelpers;


    @Override
    public void measure_setup() {
        logTrace("DC_Measurement", "measure_setup");
    }

    public DC_Measurement applyShutdown(boolean v) {
        _applyShutdown = v;
        return this;
    }

    public DC_Measurement shutdownPattern(String v) {
        _shutdownPattern = v;
        return this;
    }

    public DC_Measurement checkShutdown(int v) {
        _checkShutdown = v;
        return this;
    }

    public DC_Measurement measure(MEAS volt) {
        _measure = volt;
        return this;
    }

    public DC_Measurement settlingTime(double v) {
        _settlingTime = v;
        return this;
    }

    public DC_Measurement averages(int v) {
        _averages = v;
        return this;
    }

    public DC_Measurement VclampLow(double v) {
        _VclampLow = v;
        return this;
    }

    public DC_Measurement VclampHigh(double v) {
        _VclampHigh = v;
        return this;
    }

    public DC_Measurement highAccuracy(boolean v) {
        _highAccuracy = v;
        return this;
    }

    public DC_Measurement limitsLow(double v) {
        _limitsLow = v;
        return this;
    }

    public DC_Measurement limitsHigh(double v) {
        _VclampHigh = v;
        return this;
    }

    public DC_Measurement pin(String v) {
        _pin = v;
        return this;
    }

    public DC_Measurement forceValue(double v) {
        _forceValue = v;
        return this;
    }

    public DC_Measurement iRange(double v) {
        _iRange = v;
        return this;
    }

    public DC_Measurement badc(int v) {
        _badc = v;
        return this;
    }

    // When set to 0 the results of the test will not be judged or logged
    public DC_Measurement processResults(boolean v) {
        _processResults = v;
        return this;
    }

    public MultiSiteDoubleArray getData() {
        return _data;
    }


    public MultiSiteDouble prepForjudgeAndDatalog(MultiSiteDoubleArray result,int wordNr) {
        logTrace("DC_Measurement", "prepForjudgeAndDatalog");
        // Loop through the sites to get the data
        MultiSiteDouble MSD = new MultiSiteDouble();

        for (int site : context.getActiveSites()) {
            if(result.get(site).length > 1) {
                message(1,"Warning: not all measurements are logged, only the first one, change in DC_Measurement.java to include more");
            }
            MSD.set(site, result.get(site)[0]);
        }

        MSD = filterResult(MSD);

        return MSD;
    }

    //Convienence functions
    public MultiSiteDouble prepForjudgeAndDatalog(int wordNr) {
        return prepForjudgeAndDatalog(getData(),wordNr);
    }
    public MultiSiteDouble prepForjudgeAndDatalog() {
        return prepForjudgeAndDatalog(getData(),1);
    }

    public MultiSiteDouble logParam(IParametricTestDescriptor t) {
        MultiSiteDouble MSD = origen.prepForjudgeAndDatalog();
        t.evaluate(MSD);
        MultiSiteBoolean pf = t.getPassFail();
        for (int siteNumber : context.getActiveSites()) {
                message(Origen.LOG_PARAM,"[" + siteNumber + "](" + t.getTestName() + ") " + MSD.get(siteNumber) + " : " + (pf.get(siteNumber)? "PASSED" : "FAILED"));
        }
        return MSD;
    }

    private MultiSiteDoubleArray capturedData() {
        logTrace("DC_Measurement", "capturedData");
        // protect results to be not overwritten
        IDigInOutActionResults actionResults = measurement.digInOut(_pin).preserveActionResults();
        releaseTester();
        if (_measure == MEAS.VOLT) {
            IIforceVmeasResults ifvmResults = actionResults.iforceVmeas(_actionName);
            return ifvmResults.getVoltage(_pin);
        }

        //CURRMEAS
        IVforceImeasResults ifvmResults = actionResults.vforceImeas(_actionName);
        return ifvmResults.getCurrent(_pin);

    }


    @Override
    public void _setup() {
        message(10, "DC_Measurement --> _Setup");
        origen = this;
        pin("NVM_ANALOGIO");
        applyShutdown(true);
        checkShutdown(1);
        settlingTime(1000*1e-6);
        forceValue(0);
        iRange(0);
        VclampLow(0);
        VclampHigh(3);
        highAccuracy(true);
        limitsHigh(3);
        limitsLow(0);
        averages(1);
        processResults(true);
        badc(0);
        shutdownPattern("");


        // Call setup from the testmethod
        measure_setup();

        message(Origen.LOG_METHODTRACE, "Setting up " + _measure + " measurement");

        if (_measure == MEAS.VOLT) {
            _actionName = "iforceVmeas";
        } else {
            _actionName = "vforceImeas";
        }


        if (_applyShutdown) {
            if (_shutdownPattern.isEmpty()) {
                //TODO Fix the relative paths for the shutdownpattern
                shutdownPattern(measurement.getPatternName() + "_part1");
                String workingDirectory = System.getProperty("user.dir");
                String[] parts = _shutdownPattern.split("\\.");
                String file = workingDirectory + "/src/patterns/" + parts[2] + ".pat";
                File f = new File(measurement.getPatternName());
                if (f.exists()) {
                    message(Origen.LOG_METHODTRACE, "Shutdown pattern " + file + " found, using it.");
                } else {
                    message(Origen.LOG_FAIL, "Shutdown pattern " + file
                            + " not found, NOT using a shutdown pattern!");
                    applyShutdown(false);
                    shutdownPattern("");
                }
            }
        }

        // Create an action sequence for the measurement
        IDeviceSetup ds = DeviceSetupFactory.createInstance();
        ds.importSpec(measurement.getSpecificationName());

//        ISetupDigInOut dcVISetup = ds.addDigInOut("NVM_ANA_PIN");
//        forceValue = I_force; // The forced current
//        waitTime = 1000us; // A delay between forcing the current and measuring the voltage
//        vclampHigh = 3V; // The upper-limit of the voltage clamp
//        vclampLow = 0.0V; // The lower-limit of the voltage clamp
//        highAccuracy = true; // The highAccuracy mode of the digInOut instrument is turned off in this example
//        limits.low = 0V; // The lower-limit for the measurment to pass
//        limits.high = 3V; // The upper-limit for the measurement to pass
//
//
//        IIforceVmeas iforceVmeas = dcVISetup.iforceVmeas("iforceVmeas");
//        iforceVmeas.setForceValue(0);
//        iforceVmeas.setVclampHigh(1);
//        iforceVmeas.setVclampLow(-1);
//        iforceVmeas.setAverages(_averages);
//        iforceVmeas.setWaitTime(_settlingTime)
//
//        action iforceVmeas iforceVmeas {
//            forceValue = I_force; // The forced current
//            waitTime = 1000us; // A delay between forcing the current and measuring the voltage
//            vclampHigh = 3V; // The upper-limit of the voltage clamp
//            vclampLow = 0.0V; // The lower-limit of the voltage clamp
//            highAccuracy = true; // The highAccuracy mode of the digInOut instrument is turned off in this example
//            limits.low = 0V; // The lower-limit for the measurment to pass
//            limits.high = 3V; // The upper-limit for the measurement to pass
//
//        }
//        action vforceImeas vforceImeas {
//            forceValue = V_force; // The forced voltage
//            waitTime = 1.5ms; // A delay between forcing the voltage and measuring the current
//            irange = 0.1uA; // The expected range of the measured current
//            highAccuracy = false; // The highAccuracy mode of the digInOut instrument is turned off in this example
//            limits.low = -20.0nA; // The lower-limit for the measurment to pass
//            limits.high = -5.0nA; // The upper-limit for the measurement to pass
//
//        }


        // ds.parallelBegin("MeasurementSequence");
        // {
        ds.sequentialBegin("PatAndShutdownSeq");
        {
            ds.patternCall(measurement.getPatternName());
            ds.actionCall(_actionName);
            if (_applyShutdown) {
                ds.patternCall(_shutdownPattern);
            }
        }
        ds.sequentialEnd();
        // }
        // ds.parallelEnd();

        measurement.setSetups(ds);

    }

    @Override
    public void run() {

        logTrace("DC_Measurement", "run");
        super.run();
        measurement.execute();
        _data = capturedData();

        MultiSiteBoolean passFail = measurement.hasPassed();
        MultiSiteBoolean alwaysPass = new MultiSiteBoolean(true);

        IMeasurementResult measurementResult = measurement.preserveResult();

        // Log the functional results
        for (int siteNumber : context.getActiveSites()) {
                message(Origen.LOG_FUNC,"[" + siteNumber + "] : " + (measurement.hasPassed(siteNumber)? "PASSED" : "FAILED"));
        }

        if(forcePass) {
            FUNC.evaluate(alwaysPass);
        } else {
            FUNC.evaluate(measurementResult);
        }

        // testDescriptor.evaluate(measurementResult);
    }


    public MultiSiteDouble filterResult(MultiSiteDouble MSD) {
        return MSD;
    }

}
