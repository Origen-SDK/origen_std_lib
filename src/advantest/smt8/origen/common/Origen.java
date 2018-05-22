package origen.common;

import java.util.List;

import xoc.dta.ITestContext;
import xoc.dta.datatypes.MultiSiteLong;
import xoc.dta.datatypes.MultiSiteString;
import xoc.dta.measurement.IMeasurement;
import xoc.dta.setupaccess.IPattern;
import xoc.dta.signals.ISignal;

public class Origen {

    /** Define the different loglevels <br/>
     * Internally the SMT8 logging system will be used (message())
     * the constants here just work as shorthand for default use cases
     */
    public static final int LOG_METHODTRACE = 9;
    public static final int LOG_DATA = 8;
    public static final int LOG_PARAM = 3;
    public static final int LOG_FUNC = 4;
    public static final int LOG_FAIL = 2;
    public static final int LOG_WARNING = 1;
    public static final int LOG_ERROR = 0;

    // All general multisite variables
    // The booleans keep track of their state, these are not multisite since we only use multisite
    // assignments for the _vars
    private static MultiSiteString _lotid;
    private static boolean _lotidSet;

    private static MultiSiteLong _wafer;
    private static boolean _waferSet;

    private static MultiSiteLong _x;
    private static boolean _xSet;

    private static MultiSiteLong _y;
    private static boolean _ySet;

    private static MultiSiteLong _blockCycles;
    private static boolean _blockCyclesSet;

    private static MultiSiteLong _pageCycles;
    private static boolean _pageCyclesSet;

    public static ITestContext context;
    public static IMeasurement meas;

    /**
     * Implementing the SMT8 message system also for this non-TM class.
     * Perhaps not the prettiest thing to do but it fixes the logging.
     * The context needs to be set from a TM (like Base)
     * @param level
     * @param msg
     */
    @SuppressWarnings("deprecation")
    public static void message(int level, String msg) {
        context.message(level,msg);
    }


    public Origen() {
        _lotidSet = false;
        _waferSet = false;
        _xSet = false;
        _ySet = false;
    }

    /**
     * Returns the IPattern from the name of the pattern
     *
     * @param patname
     * @return
     */
    public static IPattern getPat(String patname) {
        return context.pattern(patname);
    }




    /**
     * Patch data per site
     *
     * @param subroutinePattern
     *            Name of the pattern to patch
     * @param pin
     *            Name of the pin to use
     * @param decData
     *            The data to patch with
     * @param size
     *            The amount of vectors to patch
     */
    public static void overlaySubroutinePerSite(String subroutinePattern, String pin,
        MultiSiteLong decData, int size) {
        message(Origen.LOG_METHODTRACE, "Performing patching on " + subroutinePattern);
        OrigenHelpers.overlaySubroutine(getPat(subroutinePattern), Origen.pinFromGroup(pin), decData, size,
                context.getActiveSites());
    }

    /**
     * Patch the same data to all sites
     *
     * @param subroutinePattern
     *            Name of the pattern to patch
     * @param pin
     *            Name of the pin to use
     * @param decData
     *            The data to patch with
     * @param size
     *            The amount of vectors to patch
     */
    public static void overlaySubroutineAllSites(String subroutinePattern, String pin, long decData,
            int size) {
        message(Origen.LOG_METHODTRACE, "Performing patching on " + subroutinePattern);
        OrigenHelpers.overlaySubroutine(getPat(subroutinePattern), Origen.pinFromGroup(pin), decData, size);
    }


    /**
     * Get 1 pin name from a pingroup. Throws an exception if the group contains more than 1 pin
     *
     * @param group
     * @return
     */
    public static String pinFromGroup(String group) {
        List<ISignal> dutGroup = meas.getSignalGroup(group).getDutSignals();
        if (dutGroup.size() > 1) {
            throw new NumberFormatException("Can only patch one signal at a time");
        }
        return dutGroup.get(0).toString();
    }

    /**
     * Debug function to display the STDF variables to check if they are properly setup
     */
    public static void showDieInfoFromSTDF() {
      Long x_coord =context.testProgram().variables().getLong("STDF.X_COORD").get();
      Long y_coord =context.testProgram().variables().getLong("STDF.Y_COORD").get();
      String wafer_id =context.testProgram().variables().getString("STDF.WAFER_ID").get();
      String lot_id =context.testProgram().variables().getString("STDF.LOT_ID").get();
      String sublot_id =context.testProgram().variables().getString("STDF.SBLOT_ID").get();
      message(Origen.LOG_DATA,"Wafer_id:" + wafer_id);
      message(Origen.LOG_DATA,"Lot_id:" + lot_id);
      message(Origen.LOG_DATA,"Sublot_id:" + sublot_id);
      message(Origen.LOG_DATA,"(x,y):(" + x_coord + "," + y_coord + ")");
    }

    /** Set the lot ID to the given value, e.g. "ABC1234" */
    public static void lotid(MultiSiteString v) {
        _lotid = v;
        _lotidSet = true;

    }

    /** Convert long value to 8 character ASCII string */
    private static String toASCII(long value) {
        int length = 8;
        StringBuilder builder = new StringBuilder(length);
        for (int i = length - 1; i >= 0; i--) {
            builder.append((char) ((value >> (8 * i)) & 0xFF));
        }
        return builder.toString();
    }

    /** Set the lot ID based on the integer representation returned from lotidInt */
    public static void lotid(MultiSiteLong v) {
        MultiSiteString lotStr = new MultiSiteString("");
        for (int site : v.getActiveSites()) {
            long val = v.get(site);
            lotStr.set(site, toASCII(val));
        }
        lotid(lotStr);
    }

    /**
     * Get the lot ID. If it has not previously been set to a value it will be automatically queried
     * from the test system.
     */
    public static MultiSiteString lotid() {
        if (!_lotidSet) {
            // TODO Don't know how to get LOTID in SMT8
            // char value[CI_CPI_MAX_MODL_STRING_LEN * 2];
            // if (!GetModelfileString(const_cast<char*>("LOT_ID"), value)) {
            // _lotid = (string) value;
            // } else {
            _lotid = new MultiSiteString("Undefine");
        }
        _lotidSet = true;

        return _lotid;
    }

    /**
     * Returns the lot ID as a 64-bit integer that is suitable for programming to the device. Each
     * character in the lotID is converted to its ASCII code. An error will be raised if the length
     * of the current lotID overflows 64-bits.
     */
    public static MultiSiteLong lotidInt() {
        MultiSiteLong tempLong = new MultiSiteLong();
        MultiSiteString lotid = lotid();
        for (int site : lotid.getActiveSites()) {
            String id = lotid.get(site);
            // If the ID is > 8 chars then lose the upper chars, making the assuming that the lower
            // ones are
            // the more significant ones for the purposes of identifying a particular lot
            if (id.length() > 8) {
                id = id.substring(id.length() - 8, 8);
            }
            // Convert each character to ascii using 2 hex (8 bytes)
            StringBuilder sb = new StringBuilder();
            char[] letters = id.toCharArray();
            for (char ch : letters) {
                sb.append(Long.toHexString(ch));
            }
            // Convert the hex string to long again
            tempLong.set(site, Long.parseLong(sb.toString(), 16));
        }
        return tempLong;
    }

    /** Set the wafer number to the given value */
    public static void wafer(MultiSiteLong valMSL) {
        for (int site : valMSL.getActiveSites()) {
            long val = valMSL.get(site);
            if (val < 0 || val > 255) {
                throw new Error("ERROR: Wafer is out of the range of a UInt8: " + val);
            }
            _wafer.set(site, val);
        }
        _waferSet = true;
    }

    /**
     * Get the wafer number. If it has not previously been set to a value it will be automatically
     * queried from the test system.
     */
    public static MultiSiteLong wafer() {
        if (!_waferSet) {
            // TODO Don't know how to get WAFER in SMT8
//            Long x_coord =context.testProgram().variables().getLong("STDF.X_COORD").get();
//            Long y_coord =context.testProgram().variables().getLong("STDF.Y_COORD").get();
//            String wafer_id =context.testProgram().variables().getString("STDF.WAFER_ID").get();
//            String lot_id =context.testProgram().variables().getString("STDF.LOT_ID").get();
//            String sublot_id =context.testProgram().variables().getString("STDF.SBLOT_ID").get();
//            println("Wafer_id:" + wafer_id);
//            println("Lot_id:" + lot_id);
//            println("Sublot_id:" + sublot_id);
//            println("(x,y):(" + x_coord + "," + y_coord + ")");
            // char value[CI_CPI_MAX_MODL_STRING_LEN * 2];
            // if (!GetModelfileString(const_cast<char*>("WAFER_ID"), value)) {
            // Expect to return something like "AB1234-15AA", where 15 is the wafer number
            // wafer(toInt(split((string) value, '-')[1].substr(0, 2)));
            // if (!GetModelfileString(const_cast<char*>("WAFER_NUMBER"), value)) {
            // wafer(toInt((string) value));
            // } else {
            _wafer = new MultiSiteLong(66);
            // }
            _waferSet = true;
        }
        return _wafer;
    }

    /** Query the X and Y coordinates from the test system and set them for all sites. */
    public static void setXY() {
     // TODO Implement this using context.testProgram().variables().getLong("STDF.X_COORD")
//         long lx, ly;
//         Long lx =context.testProgram().variables().getLong("STDF.X_COORD").get();
//         Long ly =context.testProgram().variables().getLong("STDF.Y_COORD").get();
//         GetDiePosXYOfSite(_number, &lx, &ly);
//         if (lx < -32768 || lx > 32767) {
//         cout << "ERROR: X is out of the range of an Int16: " << lx << endl;
//         ERROR_EXIT(TM::EXIT_FLOW);
//         }
//         if (ly < -32768 || ly > 32767) {
//         cout << "ERROR: Y is out of the range of an Int16: " << ly << endl;
//         ERROR_EXIT(TM::EXIT_FLOW);
//         }
//         _x = (int) lx;
//         _y = (int) ly;
        _x = new MultiSiteLong(99);
        _y = new MultiSiteLong(101);
        _xSet = true;
        _ySet = true;
    }

    /**
     * Get the X co-ord. If it has not previously been set to a value it will be automatically
     * queried from the test system.
     */
    public static MultiSiteLong x() {
        if (!_xSet) {
            setXY();
        }
        return _x;
    }

    /**
     * Get the Y co-ord. If it has not previously been set to a value it will be automatically
     * queried from the test system.
     * @return
     */
    public static MultiSiteLong y() {
        if (!_ySet) {
            setXY();
        }
        return _y;
    }

    /** Set the X co-ordinate to the given value */
    public static void x(MultiSiteLong v) {
        _x = v;
        _xSet = true;
    }

    /** Set the Y co-ordinate to the given value */
    public static void y(MultiSiteLong v) {
        _y = v;
        _ySet = true;
    }

}
