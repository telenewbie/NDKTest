# NDKTest

## 申请大内存
Java 如果 申请的内存超过 `Runtime.getRuntime().maxMemory()` 就会 抛出 堆内存溢出 的错误
C++ 如果 申请的内存 超过  `Runtime.getRuntime().maxMemory()`  在小米手机8 上面的表现现象是：内存增长10M
在使用C++进行大内存申请的时候，可以通过 `adb shell dumpsys meminfo com.tele.ndktest -d` 可以查看到 在 `native alloc ` 是 真的涨了那么多 但是对于`total`项目却只有10M的变化 而且使用 `free` 进行内存的释放 也没有立刻表现出内存的降低

## 线程命名
Java 为线程命名 的方式
```Java
Thread t = new Thread(new Runnable() {
    @Override
    public void run() {

    }
},"newbieJava1"); // 起名字
t.setName("newbieJava2");
```
Android 为线程命名的方式
```Java
HandlerThread t = new HandlerThread("newbie");
t.start();
Handler h = new Handler(t.getLooper(), new Handler.Callback() {
    @Override
    public boolean handleMessage(Message msg) {
        return false;
    }
});
```
C++ 为线程命名的方式,C++11 的写法
```C++
std::thread t([i]{
    // 为线程命名
    prctl(PR_SET_NAME, ("newbie" + std::to_string(i)).c_str());

});
t.detach();
```
显示所有的线程名称
```Shell
cat /proc/`ps -ef |grep com.tele.ndktest |awk 'NR==1{print $2}'`/task/*/comm

# output
om.tele.ndktest
Jit thread pool
Signal Catcher
ADB-JDWP Connec
ReferenceQueueD
FinalizerDaemon
FinalizerWatchd
HeapTaskDaemon
Binder:30069_1
Binder:30069_2
Binder:30069_3
Profile Saver
Binder:intercep
RenderThread
queued-work-loo
```
通过结果可以看出如果线程执行完毕的话，则线程名称就会消失，如果把sleep的时间调长一点 那么就可以看到 在上面都会显示线程名称

## 测试链接库中含有相同名称的函数时调用的顺序

```Cmake
target_link_libraries(
        native-lib
        ${log-lib}
        first-lib  # 具有 invoke_mine 函数
        second-lib # 具有 invoke_mine 函数
        )
```
通过测试,发现 连接在前 则会被调用 [优先调用 先连接的] ，为啥不会出现符号重定义呢？


## ButterKnife
经过研读源码，大概的流程是这样的，通过`AbstractProcessor` 将标注进行处理，使用`javaPoet` 生成 `类名_ViewBinding.java` 文件,
在经过ButterKnife.bind 进行构造，在构造的时候进行绑定与赋值，

这就是为什么通过 @BindView 就可以不用写findViewById 的原因了。

使用过程注意，如果采用 kotlin开发 使用butterKnife 的情况，需要修改为如下代码：
```kotlin
@BindView(R.id.button)
lateinit var mBtn_test: Button

implementation 'com.jakewharton:butterknife:10.2.1'
kapt 'com.jakewharton:butterknife-compiler:10.2.1'
```

思考：

在做内存分析的时候，发现每次都需要执行相应的指令去查看 `adb shell dumpsys meminfo [packagename]` 来查看 ，
猜想是否可以结合按钮点击创建线索以及时间间隔来打印内存变化，
然后通过python的方式将数据进行筛选并且展示在界面上面

可能需要涉及 jni层的打桩？，将需要进行打桩的函数进行处理。java 功能打桩？
因为可能需要了解 这些情况： 多次调用同一接口[c层，可能百次千次] 增加功能[java/C] 在此情况内存的变化情况
