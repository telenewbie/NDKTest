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