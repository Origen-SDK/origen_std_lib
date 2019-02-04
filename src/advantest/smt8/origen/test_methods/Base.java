package origen.test_methods;

import origen.common.Origen;
import xoc.dta.TestMethod;
import xoc.dta.datatypes.MultiSiteBoolean;
import xoc.dta.datatypes.MultiSiteDouble;
import xoc.dta.datatypes.MultiSiteLong;
import xoc.dta.measurement.IMeasurement;
import xoc.dta.resultaccess.IMeasurementResult;
import xoc.dta.testdescriptor.IFunctionalTestDescriptor;
import xoc.dta.testdescriptor.IParametricTestDescriptor;

/**
 * Origen testmethods base class. All testmethods inherit from this class
 */
public class Base extends TestMethod {
    /** General testmethod parameters, used by all testmethods */
    public IMeasurement measurement;
    public String testName;
    // TODO Verify forcePass implementation (also check if implemented in DC_Measurements)
    public Boolean forcePass = false;
    // Sites that passed will contain a '1' if forcePass has been set, otherwise undefined
    public MultiSiteLong setOnPassFlags;
    // Sites that failed will contain a '1' if forcePass has been set, otherwise undefined
    public MultiSiteLong setOnFailFlags;
    /**
     * The log level that will be used during the execution of the TP. Change the value here to get
     * more, or less logging info
     */
    int origenLoglevel = Origen.LOG_WARNING;

    /**
     * Execute the checkparms() function? This is used to check if all testmethod parameters are
     * parsed
     */
    boolean checkParams = true;

    public void logTrace(String className, String method) {
        message(Origen.LOG_METHODTRACE, "\t" + className + "\t" + method + "()");
    }








    // ********* FLOW LOGIC BELOW **********

    /**
     * <p>
     * Overridden setup function from the testmethod
     * </p>
     * <p>
     * The order is that first the setup() function is called by SMT, then the update() function and
     * then the execute() function
     * </p>
     * <p>
     * This setup() function calls the _setup function from the underlying class (Can be
     * DC_Measurement or Function_test), depending on the type of TM These subclasses are
     * responsible for proper handling of the TM setup.
     * </p>
     */
    @Override
    public void setup() {
        Origen.context = context;
        Origen.meas = measurement;
        messageLogLevel = origenLoglevel;
        logTrace("Base", "setup");
        if (!dependenciesUnchanged()) {
          _setup();
        }
    }

    /**
     * <p>
     * Overridden update function from the testmethod
     * </p>
     * <p>
     * The order is that first the setup() function is called by SMT, then the update() function and
     * then the execute() function
     * </p>
     * <p>
     * The update() function does nothing at the moment. It acts as a placeholder because all TM's
     * should have an update function. If a TM wants the use the update() functionality from SMT it
     * can implement this function itself
     * </p>
     */
    @Override
    public void update() {
        logTrace("Base", "update");
    }

    /**
     * <p>
     * Overridden execute function from the testmethod
     * </p>
     * <p>
     * The order is that first the setup() function is called by SMT, then the update() function and
     * then the execute() function
     * </p>
     * <p>
     * The execute() function first checks if the parameters are parsed. It then calls the body()
     * function from the testmethod. This is the main function of the TM. Finally it necessary, it
     * calls the process() function from the TM.
     *
     * </p>
     */
    @Override
    public void execute() {
        logTrace("Base", "execute");

        if (checkParams) {
            checkParams();
        }

        if (forcePass) {
            setOnPassFlags = new MultiSiteLong(1);
            setOnFailFlags = new MultiSiteLong(0);
        }

        body();
    }

    /**
     * Placeholder for the checkParams function. The goal of this function is to check if all the
     * testmethod parameters that are passed to a testmethod are actually used This will make sure
     * that parameters are not ignored. Should be overridden by testmethods when needed
     */
    public void checkParams() {
        logTrace("Base", "checkParams");
    }

    public void measure_setup() {
        logTrace("Base", "measure_setup");
    }

    public void process() {
        logTrace("Base", "process");
    }

    public void _setup() {
        logTrace("Base", "_setup");
    }

    public void run() {
        logTrace("Base", "run");
    }

    public void body() {
        logTrace("Base", "body");
        run();
    }

    public void judgeAndDatalog(IFunctionalTestDescriptor t, MultiSiteBoolean passed) {
        MultiSiteBoolean allPassed = new MultiSiteBoolean(true);

        if(forcePass) {
            for (int site : context.getActiveSites()) {
                setOnPassFlags.set(site, setOnPassFlags.get(site) & (passed.get(site) ? 1 : 0));
                setOnFailFlags.set(site, setOnFailFlags.get(site) | (passed.get(site) ? 0 : 1));
            }
            // Record that this test happened to STDF, but don't know how to log the true result
            // without also causing it to fail/bin
            t.evaluate(allPassed);
        } else {
            t.evaluate(passed);
        }
        for (int site : context.getActiveSites()) {
            message(Origen.LOG_PARAM,"[" + site + "](" + t.getTestName() + ") " + " : " + (passed.get(site) ? "PASSED" : "FAILED"));
        }
    }


    public void judgeAndDatalog(IFunctionalTestDescriptor t, IMeasurementResult measurementResult) {
        MultiSiteBoolean passed = measurementResult.hasPassed();
        judgeAndDatalog(t, passed);
    }


    /**
     * Log a multisite double
     * @param t  Name of the testdescriptor
     * @param MSD
     */
    public void judgeAndDatalog(IParametricTestDescriptor t, MultiSiteDouble MSD) {

        if(forcePass) {
            boolean loLimitPresent, hiLimitPresent;
            double lo = 0;
            double hi = 0;

            if (t.getLowLimit() == null) {
                loLimitPresent = false;
            } else {
                loLimitPresent = true;
                lo = t.getLowLimit().doubleValue();
            }
            if (t.getHighLimit() == null) {
                hiLimitPresent = false;
            } else {
                hiLimitPresent = true;
                hi = t.getHighLimit().doubleValue();
            }

            for (int site : context.getActiveSites()) {
                boolean passed = true;
                double val = MSD.get(site);

                // TODO: How to handle difference between LT and LTE?
                if (loLimitPresent) {
                    if (val < lo) {
                        passed = false;
                    }
                }

                if (hiLimitPresent) {
                    if (val > hi) {
                        passed = false;
                    }
                }

                setOnPassFlags.set(site, setOnPassFlags.get(site) & (passed ? 1 : 0));
                setOnFailFlags.set(site, setOnFailFlags.get(site) | (passed ? 0 : 1));
            }

            t.setLowLimit(Double.NaN);
            t.setHighLimit(Double.NaN);
        }

        t.evaluate(MSD);

        MultiSiteBoolean pf = t.getPassFail();
        for (int site : context.getActiveSites()) {
                message(Origen.LOG_PARAM,"[" + site + "](" + t.getTestName() + ") " + MSD.get(site) + " : " + (pf.get(site)? "PASSED" : "FAILED"));
        }
    }

    /**
     * Log a multisite long
     * @param t Name of the testdescriptor
     * @param MSL
     */
    public void judgeAndDatalog(IParametricTestDescriptor t, MultiSiteLong MSL) {
        judgeAndDatalog(t, MSL.toMultiSiteDouble());
    }
}
