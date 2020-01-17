package com.example.myapplication;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.app.ActivityManager;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.os.Debug;
import android.os.Process;
import android.text.format.Formatter;
import android.util.Log;

import com.example.myapplication.utils.MemoryAlloc;
import com.litesuits.common.assist.Toastor;
import com.litesuits.common.io.FileUtils;
import com.litesuits.common.utils.MemoryUtil;
import com.litesuits.common.utils.ShellUtil;

public class MemoryActivity extends BaseActivity {

    private final static int ALLOC_SIZE = 100 * 1024;

    Toastor mToast;


    @Override
    protected void initView() {
        mToast = new Toastor(this);
        addButton("从Cpp中分配内存", v -> MemoryAlloc.allocByCXX(ALLOC_SIZE));
        addButton("查看内存大小1 通过 /proc/meminfo", v -> MemoryUtil.printMemInfo());
        addButton("查看内存大小2 [ActivityManager.getMemoryInfo]", v -> MemoryUtil.printMemoryInfo(this));
        int pid = Process.myPid();
        addButton("查看当前进程的内存大小1[/proc/" + pid + "/statm]", v -> {
//            通过命令来看
            ShellUtil.CommandResult commandResult = ShellUtil.execCommand("cat /proc/" + pid + "/statm|awk '{ print ($2- $3)*4/1024}'", false);

            mToast.showSingleLongToast("内存大小：" + commandResult.responseMsg + "M/" + commandResult.result + "/" + commandResult.errorMsg);
        });
        addButton("查看当前进程的内存大小2[getProcessMemoryInfo]", v -> {
            int[] pids = new int[]{pid};
            ActivityManager mActivityManager = (ActivityManager) this.getApplication().getSystemService(Context.ACTIVITY_SERVICE);
            Debug.MemoryInfo[] memoryInfoArray = mActivityManager.getProcessMemoryInfo(pids);
            if (memoryInfoArray != null && memoryInfoArray.length >= 1) {
                for (int i = 0; i < memoryInfoArray.length; i++) {
                    StringBuilder sb = new StringBuilder();
                    Debug.MemoryInfo memoryInfo = memoryInfoArray[i];
                    sb.append("total pss:").append(memoryInfo.getTotalPss()).append("\n");
                    sb.append("private dirty :").append(memoryInfo.getTotalPrivateDirty()).append("\n");
                    sb.append("shared dirty :").append(memoryInfo.getTotalSharedDirty()).append("\n");
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                        appendMemInfo(memoryInfo, sb);
                    }
                    mToast.showSingleLongToast(sb.toString());
                }
            }
        });
        addButton("从Android中分配内存", v -> MemoryAlloc.allocByJava(ALLOC_SIZE));
        addButton("从Android中分配Bitmap内存", v -> MemoryAlloc.allocByBitmap(ALLOC_SIZE));
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void appendMemInfo(Debug.MemoryInfo memoryInfo, StringBuilder sb) {
        sb.append("private clean :").append(memoryInfo.getTotalPrivateClean()).append("\n");
    }
}
