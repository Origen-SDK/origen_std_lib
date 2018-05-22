package origen.test_methods;

import origen.common.Origen;
import xoc.dta.TestMethod;
import xoc.dta.measurement.IMeasurement;
import xoc.dta.testdescriptor.IFunctionalTestDescriptor;
import xoc.dta.testdescriptor.IParametricTestDescriptor;

/**
 * Origen testmethods base class. All testmethods inherit from this class
 */
public class Base extends TestMethod {

    /** Execute the process() function or not */
    boolean _processResults;

    /** General testdescriptors used by all testmethods */
    public IFunctionalTestDescriptor FUNC;
    public IParametricTestDescriptor PAR;

    /** General testmethod parameters, used by all testmethods */
    public IMeasurement measurement;
    public String testName;
    public Boolean forcePass = false;
    public String onPassFlag;
    public String onFailFlag;

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
        _setup();
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

        body();

        if (_processResults) {
            process();
        }
    }

    /**
     * Placeholder for the checkParams function. The goal of this function is to check if all the
     * testmethod parameters that are passed to a testmethod are actually used This will make sure
     * that parameters are not ignored. Should be overridden by testmethods when needed
     */
    public void checkParams() {
        logTrace("Base", "checkParams");
    };

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

}
