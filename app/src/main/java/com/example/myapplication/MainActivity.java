package com.example.myapplication;

import android.content.Intent;
import android.os.Bundle;
import android.util.SparseArray;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import com.example.myapplication.utils.PermissionUtils;

import java.security.Permissions;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
//        System.loadLibrary("google_speech_jni");
//        System.loadLibrary("Intercepter");
//        System.loadLibradry("native-lib");
//        System.loadLibrary("mystack");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ListView lv = new ListView(this);
        final List<Map<String, String>> datas = new ArrayList<>();
        addItem(datas, "C++字符集转换，确认输出非乱码", v -> jumpToActivity(CharsetUtilsActivity.class));
        addItem(datas, "DNS 测试", v -> jumpToActivity(DnsActivity.class));
        addItem(datas, "线程命名", v -> jumpToActivity(RenameThreadActivity.class));
        addItem(datas, "内存分配", v -> jumpToActivity(MemoryActivity.class));
        addItem(datas, "C++特性", v -> jumpToActivity(TestCXXInvokeActivity.class));
        addItem(datas, "proxy 代理", v -> jumpToActivity(TestCXXInvokeActivity.class));
        addItem(datas, "Kotlin Thread Act", v -> jumpToActivity(ThreadActivity.class));
        addItem(datas, "android common 测试", v -> jumpToActivity(ThreadActivity.class));
        addItem(datas, "堆栈测试", v -> jumpToActivity(StackActivity.class));
        addItem(datas, "hook", v -> jumpToActivity(HookActivity.class));
        addItem(datas, "stack ", v -> jumpToActivity(StackActivity.class));
        addItem(datas, "测试注解 ", v -> jumpToActivity(AnnotationActivity.class));
        addItem(datas, "测试ButterKnief ", v -> jumpToActivity(ButterKnifeActivity.class));
        addItem(datas, "日志 so打印 ", v -> jumpToActivity(LogActivity.class));
        addItem(datas, "tid vs pid", v -> jumpToActivity(PIDActivity.class));
        addItem(datas, "协程", v -> jumpToActivity(CoroutinesActivity.class));

        lv.setAdapter(new SimpleAdapter(this, datas, android.R.layout.simple_list_item_1, new String[]{"funName"}, new int[]{android.R.id.text1}));
        setContentView(lv);
        lv.setOnItemClickListener((parent, view, position, id) -> clicks.get(datas.get(position).get("funName").hashCode()).onClick(view));
    }

    SparseArray<View.OnClickListener> clicks = new SparseArray<>();

    public void addItem(List<Map<String, String>> contents, String value, View.OnClickListener listener) {
        Map<String, String> map = new HashMap<>();
        map.put("funName", value);
        clicks.put(value.hashCode(), listener);
        contents.add(map);
    }

    public void jumpToActivity(Class activityClass) {
        Intent intent = new Intent();
        intent.setClass(this, activityClass);
        intent.setPackage(getPackageName());
        MainActivity.this.startActivity(intent);
    }

    @Override
    protected void onStart() {
        super.onStart();
// 动态申请权限
//        PermissionUtils.checkPermission(this, "android.permission.RECORD_AUDIO", 100);
        PermissionUtils.checkPermission(this, "android.permission.WRITE_EXTERNAL_STORAGE", 101);
        PermissionUtils.checkPermission(this, "android.permission.READ_EXTERNAL_STORAGE", 102);
    }
}
