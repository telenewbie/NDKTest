package com.example.myapplication.utils;

import android.content.Context;
import android.os.Environment;
import android.os.storage.StorageManager;
import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class SDUtils {

    public static List<String> getAllExternalSdcardPath() {
        List<String> PathList = new ArrayList<>();

        String firstPath = Environment.getExternalStorageDirectory().getPath();
        Log.d("telenewbie", "getAllExternalSdcardPath , firstPath = " + firstPath);

        try {
            // 运行mount命令，获取命令的输出，得到系统中挂载的所有目录
            Runtime runtime = Runtime.getRuntime();
            Process proc = runtime.exec("mount");
            InputStream is = proc.getInputStream();
            InputStreamReader isr = new InputStreamReader(is);
            String line;
            BufferedReader br = new BufferedReader(isr);
            while ((line = br.readLine()) != null) {
                // 将常见的linux分区过滤掉
                if (line.contains("proc") || line.contains("tmpfs") || line.contains("media ") || line.contains("asec") || line.contains("secure") || line.contains("system") || line.contains("cache")
                        || line.contains("sys") || line.contains("data") || line.contains("shell") || line.contains("root") || line.contains("acct") || line.contains("misc") || line.contains("obb")) {
                    continue;
                }

                // 下面这些分区是我们需要的
                if (line.contains("fat") || line.contains("fuse") || (line.contains("ntfs"))) {
                    // 将mount命令获取的列表分割，items[0]为设备名，items[1]为挂载路径
                    String[] items = line.split(" ");
                    if (items.length > 1) {
                        String path = items[1].toLowerCase(Locale.getDefault());
                        // 添加一些判断，确保是sd卡，如果是otg等挂载方式，可以具体分析并添加判断条件
                        if (!PathList.contains(path) && path.contains("sd"))
                            PathList.add(items[1]);
                    }
                }
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        if (!PathList.contains(firstPath)) {
            PathList.add(firstPath);
        }

        return PathList;
    }

    /**
     * 获取系统有多少sdcard路径
     *
     * @param context
     * @return
     */
    public static String[] getSDCardsPath(Context context) {
        StorageManager sm = (StorageManager) context
                .getSystemService(Context.STORAGE_SERVICE);
        // 获取sdcard的路径：外置和内置
        String[] paths = null;
        try {
            paths = (String[]) sm.getClass().getMethod("getVolumePaths")
                    .invoke(sm);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return paths;
    }
}
