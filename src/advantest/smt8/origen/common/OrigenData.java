package origen.common;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import xoc.dta.datatypes.MultiSiteBoolean;
import xoc.dta.datatypes.MultiSiteLong;
import xoc.dta.datatypes.MultiSiteLongArray;
import xoc.dta.resultaccess.datatypes.BitSequence.BitOrder;

/**
 * Generic data holder class
 * <p>
 * Can be used for example to store data before patching. Data elements are of type long. The
 * implementation class is responsible for mapping this data to the right format
 * <p>
 * Use: It's basically a key-value pair per data point. This format is efficient for large arrays
 * with little set data. It's not so efficient for a small array with many datapoints set. The key
 * is the address and the value is called data. Everything is stored in a multisitelongarray for
 * efficient patching.
 * <p>
 * Usage example:
 *
 * <pre>
 * {@code
 *     OrigenData mem = new OrigenData();
 *     mem.setData(10, 0x55);
 *     mem.setData(12, 0x55);
 *     println("The same? " + mem.allSitesTheSame(10));
 *     mem.setDataOnSite(1, 10, 11);
 *     println("The same? " + mem.allSitesTheSame(10));
 *
 *     mem.setDataOnSite(4, 100, 44);
 *     mem.setDataOnSite(4, 200, 44);
 *     mem.setDataOnSite(4, 300, 44);
 *     mem.setData(60, 60);
 *     mem.printData();
 *
 *     println("Addr 10 site 1 set? : " + mem.addrIsSet(1, 10));
 *     int addr = 60;
 *     if (mem.allSitesTheSame(addr)) {
 *         println(mem.getDataCommon(addr));
 *     } else {
 *         println(mem.getDataMSL(addr));
 *     }
 * }
 * </pre>
 */

public class OrigenData {
    private MultiSiteLongArray mem_addr;
    private MultiSiteLongArray mem_data;

    private ArrayList<Long> _sortedUniqueElements;
    // For printing purposes only
    public int bitPerDataElement = 32;

    private boolean _anythingSet = false;
    /**
     * Constructor, initialized empty address and data arrays
     */
    public OrigenData() {
        mem_addr = new MultiSiteLongArray();
        mem_data = new MultiSiteLongArray();
        _anythingSet = false;
    }

    public ArrayList<Long> getUniqueAddressList() {
        ArrayList<Long> list = new ArrayList<Long>();
        for (int site : mem_data.getActiveSites()) {
            long[] a = mem_addr.get(site);
            for (int i = 0; i < a.length; i++) {
                if(!list.contains(a[i])) {
                    list.add(a[i]);
                }
            }
        }
        Collections.sort(list);
        return list;
    }

    public MultiSiteBoolean getActiveSitesOnAddr(long addr) {
        MultiSiteBoolean MSB = new MultiSiteBoolean();
        for (int site : mem_data.getActiveSites()) {
            MSB.set(site,addrIsSet(site, addr));
        }
        return MSB;
    }
    public int getUniqueAddr(int index) {
        _sortedUniqueElements = getUniqueAddressList();
        return _sortedUniqueElements.get(index).intValue();
    }

    public int getNumUniqueAddr() {
        _sortedUniqueElements = getUniqueAddressList();
        return _sortedUniqueElements.size();
    }


    public static <T extends Comparable<T>> List<Integer> sortIndex(final List<T> in) {
        ArrayList<Integer> index = new ArrayList<>();
        for (int i = 0; i < in.size(); i++) {
            index.add(i);
        }

        Collections.sort(index, new Comparator<Integer>() {
            @Override
            public int compare(Integer idx1, Integer idx2) {
                return in.get(idx1).compareTo(in.get(idx2));
            }
        });

        return index;
    }


    public void sort() {
        for (int site : mem_data.getActiveSites()) {
            long[] a = mem_addr.get(site);
            long[] d = mem_data.get(site);

            List<Long> list = new ArrayList<Long>(a.length);
            for (long n : a) {
                list.add(n);
            }

            List<Integer> idx = sortIndex(list);

            long[] newA = a.clone();
            long[] newD = d.clone();
            for(int i = 0; i < a.length; i++) {
                newA[i] = a[idx.get(i)];
                newD[i] = d[idx.get(i)];
            }
            mem_addr.set(site,newA);
            mem_data.set(site,newD);
        }
    }
    public boolean memEmpty() {
        return _anythingSet;
    }
    /**
     * Set data on 1 specific site for 1 address
     *
     * @param site
     * @param addr
     * @param data
     */
    public void setDataOnSite(int site, long addr, long data) {
        long[] a = mem_addr.get(site);
        long[] d = mem_data.get(site);
        if(a == null) {
            a = new long[0];
        }
        if(d == null) {
           d = new long[0];
        }
        int loc = valInAddr(a, addr);
        if (loc == d.length) {
            d = expand(d);
            a = expand(a);
        }
        d[loc] = data;
        a[loc] = addr;
        mem_data.set(site, d);
        mem_addr.set(site, a);
        _anythingSet = true;
    }


    /**
     * Set the data for a certain 32bit addr
     *
     * @param addr
     * @param data
     */
    public void setData(long addr, long data) {
        for (int site : mem_data.getActiveSites()) {
            setDataOnSite(site, addr, data);
        }
    }

    /**
     * Returns true if a certain address is set on ANY site
     *
     * @param addr
     * @return
     */
    public boolean addrIsSetAnySite(long addr) {
        for (int site : mem_data.getActiveSites()) {
            if (addrIsSet(site, addr)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Returns true if a certain address is set on a specific site
     *
     * @param site
     * @param addr
     * @return
     */
    public boolean addrIsSet(int site, long addr) {
        long[] a = mem_addr.get(site);
        int loc = valInAddr(a, addr);
        return loc != a.length;
    }

    /**
     * Returns the common data for all sites. Throws an error if this specific address is
     * site-specific
     *
     * @param addr
     * @return
     */
    public long getDataCommon(long addr) {
        int sites[] = mem_data.getActiveSites();
        if (allSitesTheSame(addr)) {
            return getDataPerSite(sites[0], addr);
        }
        throw new Error(
                "Not all sites have the same data, cannot give common data for this addr: " + addr);
    }

    /**
     * Returns the site specific data for an address, returning -1 for the data if it
     * has not been previously set
     *
     * @param addr
     * @return
     */
    public MultiSiteLong getDataMSL(long addr) {
      return getDataMSL(addr, false, "");
    }

    /**
     * Returns the site specific data for an address, but raising and error with the given
     * message if the data has not been previously set
     *
     * @param addr
     * @return
     */
    public MultiSiteLong getDataMSL(long addr, String errorMsg) {
      return getDataMSL(addr, true, errorMsg);
    }

    private MultiSiteLong getDataMSL(long addr, boolean errorOnNotSet, String errorMsg) {
        MultiSiteLong result = new MultiSiteLong();
        for (int site : mem_data.getActiveSites()) {
            if (addrIsSet(site, addr)) {
                result.set(site, getDataPerSite(site, addr));
            } else {
                if (errorOnNotSet) {
                  throw new Error(errorMsg);
                }
                result.set(site, -1);
            }
        }
        return result;
    }

    /**
     * Returns whether or not all the sites have the same data for this addr
     *
     * @param addr
     * @return
     */
    public boolean allSitesTheSame(long addr) {
        long commonData = -1;
        boolean addrFound = false, addrNotFound = false;
        for (int site : mem_data.getActiveSites()) {
            long[] d = mem_data.get(site);
            long[] a = mem_addr.get(site);
            int loc = valInAddr(a, addr);
            if (loc != a.length) {
                if(addrNotFound) {
                    return false;
                }
                // Addr is found
                addrFound = true;
                if (commonData == -1) {
                    commonData = d[loc];
                } else {
                    if (commonData != d[loc]) {
                        // Not all data the same for this addr over all sits
                        return false;
                    }
                }
            } else {
                addrNotFound = true;
                // Addr is found on one site but not the other
                if(addrFound) {
                    return false;
                }
            }
        }
        return true;
    }


    /**
     * Print all set data for all sites
     */
    public void printData() {
        sort();
        System.out.println(getUniqueAddressList());
        for (int site : mem_data.getActiveSites()) {
            System.out.println("Site: " + site);
            long[] d = mem_data.get(site);
            long[] a = mem_addr.get(site);
            for (int i = 0; i < d.length; i++) {
                System.out.println(a[i] + "\t" + OrigenHelpers.longToPaddedHexString(d[i],
                        bitPerDataElement / 4, BitOrder.RIGHT_TO_LEFT));
            }
        }
    }

    /**
     * Clears all data on all sites
     */
    public void clearAllData() {
        // Lazy man's approach: Basically just throwing the reference to the old MSLarray away
        // Let's hope the garbage collector removes the old references nicely
        mem_addr = new MultiSiteLongArray();
        mem_data = new MultiSiteLongArray();
        _anythingSet = false;

    }
    // Some private helper functions

    /**
     * Returns the location in the array for a certain address. Returns the last+1 location of the
     * array if addr is not found
     *
     * @param arr
     * @param val
     * @return
     */
    private int valInAddr(long[] arr, long val) {
        if(arr != null) {
            for (int i = 0; i < arr.length; i++) {
                if (val == arr[i]) {
                    return i;
                }
            }
            return arr.length;
        }
        return 0;
    }

    /**
     * Get the data for a specific site. This is private because the testmethod should call
     * getDataMSL()
     *
     * @param site
     * @param addr
     * @return
     */
    private long getDataPerSite(int site, long addr) {
        long[] d = mem_data.get(site);
        long[] a = mem_addr.get(site);
        int loc = valInAddr(a, addr);
        return d[loc];
    }

    /**
     * Expand the array by 1
     *
     * @param origArray
     * @return
     */
    private long[] expand(long[] origArray) {
        long[] newArray = new long[origArray.length + 1];
        System.arraycopy(origArray, 0, newArray, 0, origArray.length);
        return newArray;
    }

}

class DataPair<T> {
    private final T addr;
    private final T data;

    public DataPair(T first, T second) {
        addr = first;
        data = second;
    }

    public T addr() {
        return addr;
    }

    public T second() {
        return data;
    }
}
