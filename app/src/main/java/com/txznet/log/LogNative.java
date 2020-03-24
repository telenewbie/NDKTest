package com.txznet.log;

public class LogNative {

    public static native long setLogLevel(int level);

    public static native boolean isEnableLevel(long ptrId, int level);


    public static native void write(long ptrId, long pid, long tid, int logId, byte[] function, byte[] file, int line, int level, byte[] tag, byte[] msg);

    public static native void writeNoCheckLevel(long ptrId, long pid, long tid, int logId, byte[] function, byte[] file, int line, int level, byte[] tag, byte[] msg);

}
