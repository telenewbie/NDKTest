package com.example.myapplication;

import android.content.Intent;
import android.os.Bundle;
import android.util.SparseArray;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("mystack");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ListView lv = new ListView(this);
        final List<Map<String, String>> datas = new ArrayList<>();
        addItem(datas, "线程命名", v -> jumpToActivity(RenameThreadActivity.class));
        addItem(datas, "内存分配", v -> jumpToActivity(MemoryActivity.class));
        addItem(datas, "C++特性", v -> jumpToActivity(TestCXXInvokeActivity.class));
        addItem(datas, "proxy 代理", v -> jumpToActivity(TestCXXInvokeActivity.class));
        addItem(datas, "Kotlin Thread Act", v -> jumpToActivity(ThreadActivity.class));
        addItem(datas, "android common 测试", v -> jumpToActivity(ThreadActivity.class));
        addItem(datas, "堆栈测试", v -> jumpToActivity(StackActivity.class));
        addItem(datas, "hook", v -> jumpToActivity(HookActivity.class));
        addItem(datas, "stack ", v -> jumpToActivity(StackActivity.class));

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


}
