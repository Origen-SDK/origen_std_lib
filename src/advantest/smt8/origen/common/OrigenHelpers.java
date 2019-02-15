package origen.common;

import xoc.dta.datatypes.MultiSiteLong;
import xoc.dta.resultaccess.datatypes.BitSequence;
import xoc.dta.resultaccess.datatypes.BitSequence.BitOrder;
import xoc.dta.setupaccess.IPattern;
import xoc.dta.setupaccess.IStateCharBuffer;


public class OrigenHelpers{

    public static boolean isEven(int number) {
        return (number % 2) == 0;
    }

    public static boolean isOdd(int number) {
        return (number % 2) != 0;
    }

    public static boolean isEven(long number) {
        return (number % 2) == 0;
    }

    public static boolean isOdd(long number) {
        return (number % 2) != 0;
    }

    /**
     * String parameter to assert it's not used
     * Default value is "0"
     * @param params
     */
    public static void assertNotImplStrParam(String params) {
        assertNotImplementedStringParam(new String[] {params},"0");
    }

    /**
     * Check list of string parameters to assert they are not used
     * Default value is "0"
     * @param params
     */
    public static void assertNotImplStrParam(String[] params) {
        assertNotImplementedStringParam(params,"0");
    }

    /**
     * Check list of string parameters to make sure they are not used
     * @param params
     * @param defValue
     */
    public static void assertNotImplementedStringParam(String[] params, String defValue) {
        for(String p : params) {
            if (p != null && (p != "" && p != defValue)) {
                throw new Error("String Paramvalue: " + p + " is not defined!!");
            }
        }
    }

    /**
     * Check list int parameters to make sure they not used
     * Default value = 0
     * @param params
     */
    public static void assertNotImplIntParam(int[] params) {
        assertNotImplementedIntParam(params,0);
    }

    /**
     * Check list of int parameter to make sure it's not used
     * @param params
     * @param defValue
     */
    public static void assertNotImplementedIntParam(int[] params, int defValue) {
        for(int p : params) {
            if (p != 0) {
                throw new Error("Int Paramvalue: " + p + " is not defined!!");
            }
        }
    }

    /**
     * Check double paramter they make sure it's not used
     * @param params
     */
    public static void assertNotImplDoubleParam(double params) {
        assertNotImplDoubleParam(new double[] {params});
    }

    /**
     * Check list of double parameters to make sure they are not used
     * Default value = 0
     * @param params
     */
    public static void assertNotImplDoubleParam(double[] params) {
        assertNotImplementedDoubleParam(params,0);
    }

    /**
     * Check list of double parameters
     * @param params
     * @param defValue
     */
    public static void assertNotImplementedDoubleParam(double[] params, double defValue) {
        for(double p : params) {
            if (p != 0) {
                throw new Error("double Paramvalue: " + p + " is not defined!!");

            }
        }
    }

 // Overlays the given data on the given pin, starting from the first vector of the given pattern
    //For ALL sites the same
    public static void overlaySubroutine(IPattern subroutinePattern, String pin, long decData, int size) {
        if (size > 64) {
            throw new NumberFormatException("Cannot patch more than 64 bits using Long");
        }
        if(decData != -1) {
            String data=  longToPaddedBinaryString(decData,size, BitOrder.LEFT_TO_RIGHT, false);
            subroutinePattern.vector(0).writeStateChar(data.toCharArray(), pin);
        }
    }

    public static String longToPaddedHexString(Long data, int size, BitSequence.BitOrder order) {
        String binStr = longToPaddedBinaryString(data, size, order, false);
        long value = Long.parseLong(binStr, 2);
        String format = "%" + size + "s";
        String strData = String.format(format, Long.toHexString(value).toUpperCase()).replace(" ", "0");
        return strData;

    }
    public static String longToPaddedBinaryString(Long data, int size, BitSequence.BitOrder order, boolean replace01ToHL) {
        // Enforce minimum string size
        String format = "%" + size + "s";
        String strData = String.format(format, Long.toBinaryString(data)).replace(" ", "0");
        if(order == BitOrder.LEFT_TO_RIGHT) {
            strData = new StringBuilder(strData).reverse().toString();
//            System.out.println(strData);
         // Enforce maximum string size
            strData = strData.substring(0,size);
        }


        if(replace01ToHL) {
            strData = strData.replace("0", "L").replace("1","H");
        }
        return strData;

    }
    //Overlay per site
    public static void overlaySubroutine(IPattern subroutinePattern, String pin, MultiSiteLong decData, int size, int[] sites) {
        if (size > 64) {
            throw new NumberFormatException("Cannot patch more than 64 bits using Long");
        }
        IStateCharBuffer scBuf = subroutinePattern.createStateCharBuffer();
        for (int site : sites) {
            long longData = decData.get(site);
            //Only patch if data != -1, this can be used to not patch data on certain sites
            if(longData != -1) {
                String data=  longToPaddedBinaryString(longData,size, BitOrder.LEFT_TO_RIGHT, false);
                scBuf.writeStateChar(site, 0, pin, data.toCharArray());
            }
        }

        scBuf.flush();
    }
}
