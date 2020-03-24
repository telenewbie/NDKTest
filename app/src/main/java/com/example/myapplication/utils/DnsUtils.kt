package com.example.myapplication.utils

class DnsUtils {
    companion object {
        init {
            System.loadLibrary("dns")
        }

        external fun getIps(domain: String): ByteArray
    }

}