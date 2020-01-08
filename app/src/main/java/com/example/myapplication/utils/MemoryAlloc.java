package com.example.myapplication.utils;

public class MemoryAlloc {

    public static native long allocByCXX(long size);

    public static void allocByJava(long size) {
    }

    public static void allocByBitmap(long size) {
    }

    public static native void freeCXX(long ptr);

    public static void freeJava() {

    }

    public static void freeBitmap() {

    }
}
