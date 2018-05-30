package origen.test_methods;

import java.util.ArrayList;
import java.util.List;

import origen.common.Origen;
import origen.common.OrigenHelpers;
import xoc.dsa.DeviceSetupFactory;
import xoc.dsa.IDeviceSetup;
import xoc.dta.datatypes.MultiSiteBoolean;
import xoc.dta.datatypes.MultiSiteDouble;
import xoc.dta.datatypes.MultiSiteLong;
import xoc.dta.datatypes.MultiSiteString;
import xoc.dta.measurement.IMeasurement;
import xoc.dta.resultaccess.IDigInOutCaptureResults;
import xoc.dta.resultaccess.IMeasurementResult;
import xoc.dta.resultaccess.datatypes.BitSequence.BitOrder;
import xoc.dta.resultaccess.datatypes.MultiSiteBitSequence;
import xoc.dta.setupaccess.IParallelGroup;
import xoc.dta.testdescriptor.IParametricTestDescriptor;

/**
 * The Function test template for all functional tests
 */
public class Functional_test extends Base {

    // Class variables

    /** Pin to capture from */
    private String _pin;

    /** Number of vectors to capture */
    private int _capture;

    /** Number of bits per captured word */
    private int _bitPerWord;

    /** Pattern name */
    private String _pattern;

    /** Testname to override */
    private String _testNameOverride;


    private MultiSiteBitSequence _data;

    // Link to self
    public Functional_test origen;

    // Patching information
    /** Do we need to activate the patched patterns? */
    boolean activatePatchedMeas;

    /** The measurement that is only used for patching */
    public IMeasurement patchedMeas;

    /** The measurement that is only used dynamic pattern switching */
    public IMeasurement dynamicPatMeas;

    IMeasurementResult dynamicMeasurementResult;
    boolean _hasDynamicMeas = false;

    /** The list of patterns to patch */
    List<String> patchList;

    /**
     * Used to select only one pattern from a list of patterns for site specific execution
     */
    List<String> dynamicPatternList;

    /** Do we want to release the tester before processing? */
    boolean enableReleaseTester;

    @Override
    public void _setup() {
        message(Origen.LOG_METHODTRACE, "Functional_test --> Setup");
        origen = this;
        patchList = new ArrayList<String>();
        dynamicPatternList = new ArrayList<String>();
        patchedMeas.setSpecificationName(measurement.getSpecificationName());
        dynamicPatMeas.setSpecificationName(measurement.getSpecificationName());
        pin("");
        capture(0);
        processResults(true);
        bitPerWord(1);
        pattern("");
        testName("");

        measure_setup();

        //TODO Enable releasetester is now ALWAYS true, perhaps make this a variable?
        enableReleaseTester = true;
    }

    /**
     * Creates/adds to a list of patched patterns
     * Needs to be called for every pattern that has to be patched
     * @param pat
     * @return
     */
    public Functional_test addToPatchList(String pat) {

        patchList.add("C402TNVMTester.patterns." + pat);
        return this;
    }

    public Functional_test addToDynamicPatternList(String pat) {
        dynamicPatternList.add(pat);
        return this;
    }

    public Functional_test overlaySubroutinePerSite(String subroutinePattern,
            MultiSiteLong decData, int size) {
        Origen.overlaySubroutinePerSite("C402TNVMTester.patterns." + subroutinePattern,"NVM_DIN_PIN", decData, size);
        return this;
    }

    public Functional_test overlaySubroutineAllSites(String subroutinePattern, long decData,int size) {
        Origen.overlaySubroutineAllSites("C402TNVMTester.patterns." + subroutinePattern, "NVM_DIN_PIN", decData, size);
        return this;
    }


    /**
     * Setup the patch measurement instance
     * Call after all patterns are added to the list
     * The setup creates a new devicesetup with fake pattern calls to all patched patterns
     * This make sure that they are referenced in this testmethod.
     * Is is not possible to patch patterns that are not used in the testflow
     * After this setupPatch() is called, the patchedMeas still needs to be activated, this happens in run()
     * @return
     */
    public Functional_test setupPatch() {
        IDeviceSetup ds = DeviceSetupFactory.createInstance();
        ds.importSpec(measurement.getSpecificationName());
        ds.sequentialBegin("measPatch");
        {
            for(String pat : patchList) {
                ds.patternCall(pat);
            }
        }
        ds.sequentialEnd();
        patchedMeas.setSetups(ds);

        // Warn run() that we need to activate the patched measurement still
        activatePatchedMeas = true;

        return this;
    }

    public Functional_test setupDynamicPatternList() {
        IDeviceSetup ds = DeviceSetupFactory.createInstance();
        ds.importSpec(dynamicPatMeas.getSpecificationName());
        for(String pat : dynamicPatternList) {
            ds.parallelBegin(pat);
            {
                ds.setBypassable();
                ds.patternCall("C402TNVMTester.patterns." + pat);
            }
            ds.parallelEnd();
        }
        dynamicPatMeas.setSetups(ds);
        _hasDynamicMeas = true;
        return this;
    }

    public void executeDynamicPat(String pat) {
        selectDynamicPat(pat);
        dynamicPatMeas.execute();
        dynamicMeasurementResult = dynamicPatMeas.preserveResult();
    }

    public void executeDynamicPat(String pat,MultiSiteBoolean bypassSitesMSB) {
        selectDynamicPat(pat,bypassSitesMSB);
        dynamicPatMeas.execute();
    }

    public Functional_test selectDynamicPat(String pat, MultiSiteBoolean bypassSitesMSB) {
        message(Origen.LOG_METHODTRACE, "Executing dynamic pattern: " + pat);
        boolean patFound = false;
        MultiSiteBoolean allPass = new MultiSiteBoolean(true);
        List<IParallelGroup> paraGroups = dynamicPatMeas.operatingSequence().getParallelGroups();
        for (IParallelGroup iParallelGroup : paraGroups) {
              if (iParallelGroup.getName().equals(pat)) {
                      patFound = true;
                      iParallelGroup.setBypass(bypassSitesMSB);
              } else {
                  iParallelGroup.setBypass(allPass);
              }
        }
        if(!patFound)  {
            throw new Error("Dynamic pattern: " + pat + " not found!! Make sure you have it added in setup()");
        }
        return this;
    }

    public Functional_test selectDynamicPat(String pat) {
        MultiSiteBoolean allPass = new MultiSiteBoolean(true);
        selectDynamicPat(pat, allPass.not());
        return this;
    }

    /**
     * The functional test method can optionally capture data by supplying the number of vectors to
     * capture
     * @param v Number of vectors to capture
     * @return
     */
    public Functional_test capture(int numVecs) {
        _capture = numVecs;
        return this;
    }

    /**
     * If data capture is requested, supply the pin to capture data from
     * @param pin Pin to capture from
     * @return
     */
    public Functional_test pin(String pin) {
        _pin = pin;
        return this;
    }

    /**
     * Serial capture data will be grouped into words, specify how many bits per word in the serial
     * stream (default 1)
     * @param bitsPerWord
     * @return
     */
    public Functional_test bitPerWord(int bitsPerWord) {
        _bitPerWord = bitsPerWord;
        return this;
    }


    /**
     * Override the pattern argument from the test suite
     * @param patName
     * @return
     */
    public Functional_test pattern(String patName) {
        _pattern = patName;
        return this;
    }

    /**
     * When set to 0 the results of the test will not be judged or logged
     * @param v
     * @return
     */
    public Functional_test processResults(boolean v) {
        _processResults = v;
        return this;
    }


    /**
     * Override the test name argument from the test suite, this can be useful if the main test item
     *  to be logged is a captured value and the pattern execution is to be logged separately. The
     * override is only valid for one call to execute()
     * @param testName
     * @return
     */
    public Functional_test testName(String newTestName) {
        _testNameOverride = newTestName;
        return this;
    }

    @Override
    public void process() {
        logTrace("Function_test","process");
    }

    public MultiSiteBitSequence getData() {
        return _data;
    }

    /**
     * Get the captured data
     * @return The captured bits
     */
    private MultiSiteBitSequence capturedData() {
        // protect results to be not overwritten
        IDigInOutCaptureResults digCapture = measurement.digInOut(_pin)
                .preserveCaptureResults();

        // get all results from measurement pin
        return digCapture.getBits(_pin);
    }

    /**
     * Log a multisite double
     * @param t  Name of the testdescriptor
     * @param MSD
     */
    public void logParam(IParametricTestDescriptor t, MultiSiteDouble MSD) {
        t.evaluate(MSD);
        MultiSiteBoolean pf = t.getPassFail();
        for (int siteNumber : context.getActiveSites()) {
                message(Origen.LOG_PARAM,"[" + siteNumber + "](" + t.getTestName() + ") " + MSD.get(siteNumber) + " : " + (pf.get(siteNumber)? "PASSED" : "FAILED"));
        }
    }

    /**
     * Log a multisite long
     * @param t Name of the testdescriptor
     * @param MSL
     */
    public void logParam(IParametricTestDescriptor t, MultiSiteLong MSL) {
        t.evaluate(MSL);
        MultiSiteBoolean pf = t.getPassFail();
        for (int siteNumber : context.getActiveSites()) {
                message(Origen.LOG_PARAM,"[" + siteNumber + "](" + t.getTestName() + ") " + MSL.get(siteNumber) + " : " + (pf.get(siteNumber)? "PASSED" : "FAILED"));
        }
    }

    /**
     * Log a multisite long with data retrieved from a specific word from the measurement
     * @param t  Name of the testdescriptor
     * @param wordNr Wordnumber to log
     */
    public void logParam(IParametricTestDescriptor t, int wordNr) {
        MultiSiteLong MSL = prepForjudgeAndDatalog(getData(),wordNr);
        logParam(t, MSL);
    }

    /**
     * Gets a specific word from the measurement result
     * Bitorder is fixed to RIGHT_TO_LEFT
     * @param wordNr
     * @return
     */
    public MultiSiteLong getResultWord(int wordNr) {
        message(Origen.LOG_METHODTRACE,"Get result word" + wordNr);
        return getResultWord(wordNr, BitOrder.RIGHT_TO_LEFT);
    }

    /**
     * Gets a specific word from the measurement result
     * Bitorder has to be specified
     * @param wordNr
     * @param order BitOrder (eg. RIGHT_TO_LEFT)
     * @return
     */
    public MultiSiteLong getResultWord(int wordNr, BitOrder order) {
        return prepForjudgeAndDatalog(getData(),wordNr, order);
    }


    /**
     * Gives the string representation of the binary sequence of the result data for a specific wordnr
     * Bitorder is fixed to RIGHT_TO_LEFT
     * @param wordNr
     * @return
     */
    public MultiSiteString getBinaryWord(int wordNr) {
        MultiSiteString MSS = new MultiSiteString();
        for (int site : context.getActiveSites()) {
            MSS.set(site,OrigenHelpers.longToPaddedBinaryString(_data.get(site).toLongArray(_bitPerWord,BitOrder.RIGHT_TO_LEFT)[wordNr], _bitPerWord, BitOrder.RIGHT_TO_LEFT, false));
        }
        return MSS;
    }

    /**
     * Gives the string representation of the HEX sequence of the result data for a specific wordnr
     * Bitorder is fixed to RIGHT_TO_LEFT
     * @param wordNr
     * @return
     */
    public MultiSiteString getHexWord(int wordNr,int nrHexChars) {
        MultiSiteString MSS = new MultiSiteString();
        for (int site : context.getActiveSites()) {
            MSS.set(site,OrigenHelpers.longToPaddedHexString(_data.get(site).toLongArray(_bitPerWord,BitOrder.RIGHT_TO_LEFT)[wordNr], nrHexChars, BitOrder.RIGHT_TO_LEFT));
        }
        return MSS;
    }

    /** Helper function for default bitPerWord use */
    public MultiSiteString getHexWord(int wordNr) {
        return getHexWord(wordNr,_bitPerWord/4);
    }
    /**
     * Fills the result array with captured data for a specific wordNr for all sites
     * BitOrder is fixed to RIGHT_TO_LEFT
     * @param result
     * @param wordNr
     * @return
     */
    public MultiSiteLong prepForjudgeAndDatalog(MultiSiteBitSequence result,int wordNr) {
        return prepForjudgeAndDatalog(result,wordNr, BitOrder.RIGHT_TO_LEFT);
    }

    /**
     * Fills the result array with captured data for a specific wordNr for all sites
     * BitOrder needs to specified
     * @param result
     * @param wordNr
     * @param order
     * @return
     */
    public MultiSiteLong prepForjudgeAndDatalog(MultiSiteBitSequence result,int wordNr, BitOrder order) {

        // Init the MultiSite array
        MultiSiteLong MSL = new MultiSiteLong();

        // Loop through the sites to get the data
        for (int site : context.getActiveSites()) {
            message(Origen.LOG_METHODTRACE,"Num cap words: " + result.get(site).toLongArray(_bitPerWord,order).length);
            MSL.set(site, result.get(site).toLongArray(_bitPerWord,order)[wordNr]);
        }
        return MSL;
    }

//    public MultiSiteLongArray getAllData() {
//        BitOrder order = BitOrder.RIGHT_TO_LEFT;
//        MultiSiteBitSequence result = getData();
//        for (int site : context.getActiveSites()) {
//            resultPerSite = result.get(site).toLongArray(_bitPerWord,order)
//    }
//
    public void executeAndCapture() {
        // Run the measurement
        measurement.digInOut(_pin).result().cyclePassFail().setEnabled(false);
        measurement.digInOut(_pin).result().capture().setEnabled(true);
        measurement.execute();
        _data = capturedData();
    }

    /** Main run function for functional */
    @Override
    public void run() {
        logTrace("Functional_test", "run");
        super.run();

        // Check if we need to override the pattern name
        if (_pattern != "") {
            measurement.setPatternName(_pattern);
        }

        // Check if we need to capture anything
        // When capture is enabled, we have to disable the cyclePassFail
        if (_capture > 0) {
            logTrace("Functional_test", "Enabling capture on " + _pin);
            measurement.digInOut(_pin).result().cyclePassFail().setEnabled(false);
            measurement.digInOut(_pin).result().capture().setEnabled(true);
        } else {
            logTrace("Functional_test", "Disabling capture on " + _pin);
            measurement.digInOut(_pin).result().cyclePassFail().setEnabled(true);
            measurement.digInOut(_pin).result().capture().setEnabled(false);
        }

        // Run the measurement
        measurement.execute();

        MultiSiteBoolean passFail = measurement.hasPassed();
        MultiSiteBoolean alwaysPass = new MultiSiteBoolean(true);
        IMeasurementResult measurementResult = measurement.preserveResult();

        // When captured was enabled, we need to load the captured data for later processing
        // After this is done, the tester can be released
        if (_capture > 0) {
            _data = capturedData();
        }

        // Activate the patched measurements
        // Without this, you cannot patch any patterns that are not used in THIS testmethod
        if(activatePatchedMeas) {
            patchedMeas.activate();
        }

        // next test suite can start and use tester resources
        // check, if background processing should be enabled
        if (enableReleaseTester) {
            releaseTester();
        }

        // Log the functional results
        for (int siteNumber : context.getActiveSites()) {
                message(Origen.LOG_FUNC,"[" + siteNumber + "] : " + (measurement.hasPassed(siteNumber)? "PASSED" : "FAILED"));
        }


        if(forcePass) {
            FUNC.evaluate(alwaysPass);
        } else {
            if(_hasDynamicMeas) {
                FUNC.evaluate(dynamicMeasurementResult.hasPassed().and(measurementResult.hasPassed()));
            } else {
                FUNC.evaluate(measurementResult);
            }
        }
    }

    @Override
    public void checkParams() {
     // Placeholder

    }

}
