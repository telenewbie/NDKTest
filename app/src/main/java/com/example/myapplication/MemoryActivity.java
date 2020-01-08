package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.example.myapplication.utils.MemoryAlloc;

public class MemoryActivity extends BaseActivity {

    private final static int ALLOC_SIZE = 100 * 1024;

    @Override
    protected void initView() {
        addButton("从Cpp中分配内存", v -> MemoryAlloc.allocByCXX(ALLOC_SIZE));
        addButton("从Android中分配内存", v -> MemoryAlloc.allocByJava(ALLOC_SIZE));
        addButton("从Android中分配Bitmap内存", v -> MemoryAlloc.allocByBitmap(ALLOC_SIZE));
    }
}
