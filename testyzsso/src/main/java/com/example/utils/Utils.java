package com.example.utils;

import android.content.Context;
import android.util.Log;
import android.util.Pair;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Utils {
    private static final String TAG = "Utils";
    private static final Pattern pattern = Pattern.compile(".*<s>\\s*(.*)\\s*</s>.*\\n\\s*(.*)\\s*");

    public static void copyFile(Context context, String assetsFile, String destination) {
        InputStream is = null;
        FileOutputStream fos = null;
        byte[] buf1 = new byte[512];
        try {
            File des = new File(destination, assetsFile);
            if (des.exists()) {
                return;
            }
            if (!new File(destination).exists()) {
                Log.d(TAG, "mkdirs  " + new File(destination).mkdirs());
            }
            Log.d(TAG, "copy to: " + des.getAbsolutePath());
            fos = new FileOutputStream(des);
            is = context.getAssets().open(assetsFile);
            int readCount;
            while ((readCount = is.read(buf1)) > 0) {
                fos.write(buf1, 0, readCount);
            }
            fos.flush();
        } catch (IOException e) {
            e.printStackTrace();
            Log.d(TAG, "copy: ", e);
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static StringBuffer getAssetsFile(Context context, String fileName) {
        StringBuffer sb = new StringBuffer(1024 * 10);
        try {
            InputStream in = context.getAssets().open(fileName);
            InputStreamReader reader = new InputStreamReader(in);
            BufferedReader bufferReader = new BufferedReader(reader);
            String line;
            while ((line = bufferReader.readLine()) != null) {
                sb.append(line).append("\n");
            }
            reader.close();
            in.close();
            return sb;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return sb;
    }

    /**
     * 从assets目录中复制整个文件夹内容
     *
     * @param context Context 使用CopyFiles类的Activity
     * @param oldPath String  原文件路径  如：/aa
     * @param newPath String  复制后路径  如：xx:/bb/cc
     */
    public static void copyFilesFassets(Context context, String oldPath, String newPath) {
        try {
            String fileNames[] = context.getAssets().list(oldPath);//获取assets目录下的所有文件及目录名
            if (fileNames.length > 0) {//如果是目录
                File file = new File(newPath);
                file.mkdirs();//如果文件夹不存在，则递归
                for (String fileName : fileNames) {
                    copyFilesFassets(context, oldPath + "/" + fileName, newPath + "/" + fileName);
                }
            } else {//如果是文件
                InputStream is = context.getAssets().open(oldPath);
                FileOutputStream fos = new FileOutputStream(new File(newPath));
                byte[] buffer = new byte[1024];
                int byteCount = 0;
                while ((byteCount = is.read(buffer)) != -1) {//循环从输入流读取 buffer字节
                    fos.write(buffer, 0, byteCount);//将读取的输入流写入到输出流
                }
                fos.flush();//刷新缓冲区
                is.close();
                fos.close();
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            //如果捕捉到错误则通知UI线程
//      MainActivity.handler.sendEmptyMessage(COPY_FALSE);
        }
    }
}
