package no.mechatronics.sfi.fmi4j.jni;

import no.mechatronics.sfi.fmi4j.importer.misc.OSUtil;

import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class FmiLibrary implements Closeable {

    static {

        String fileName = OSUtil.getLibPrefix() + "fmi." + OSUtil.getLibExtension();
        try (InputStream is = FmiLibrary.class.getClassLoader()
                .getResourceAsStream("native/fmi/" + OSUtil.getCurrentOS() + "/" + fileName)) {

            File copy = new File(fileName);
            copy.deleteOnExit();
            try (FileOutputStream fos = new FileOutputStream(copy)) {
                byte[] buffer = new byte[1024];
                int bytes = is.read(buffer);
                while (bytes >= 0) {
                    fos.write(buffer, 0, bytes);
                    bytes = is.read(buffer);
                }
            }
            System.load(copy.getAbsolutePath());

        } catch (Exception ex) {
            throw new RuntimeException(ex);
        }

    }


    public FmiLibrary(String libName) {
        if (!load(libName)) {
            throw new RuntimeException("Unable to load native library!");
        }
    }

    private native boolean load(String libName);

    public native void close();

    public native String getFmiVersion();

    public native String getTypesPlatform();

    public native int setDebugLogging(long c, boolean loggingOn, int nCategories, String[] categories);

    public native int setupExperiment(long c, boolean toleranceDefined, double tolerance, double startTime, boolean stopTimeDefined, double stopTime);

    public native int enterInitializationMode(long c);

    public native int exitInitializationMode(long c);

    public native long instantiate(String instanceName, int type, String guid, String resourceLocation, boolean visible, boolean loggingOn);

    public native int step(long c, double currentCommunicationPoint, double communicationStepSize, boolean noSetFMUStatePriorToCurrentPoint);

    public native int terminate(long c);

    public native int reset(long c);

    public native void fmi2FreeInstance(long c);

    public native int getInteger(long pointer, int vr);

    public native int[] getInteger(long pointer, int[] vr);

    public native double getReal(long pointer, int vr);

    public native int getReal(long pointer, int[] vr, double[] ref);

    public native String getString(long pointer, int vr);

    public native String[] getString(long pointer, int[] vr);

    public native boolean getBoolean(long pointer, int vr);

    public native boolean[] getBoolean(long pointer, int vr[]);

}
