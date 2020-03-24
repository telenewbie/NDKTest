## 关于kotlin 生成 jni的方式

编写好kotlin :
```kotlin
class DnsUtils {
    companion object {
        init {
            System.loadLibrary("dns")
        }
        external fun getIps(domain: String): ByteArray
    }
}
```
执行编译
```bat
//格式  javah -d 存放头文件的目录 -cp 加载类的目录  完整包名和类名

D:\code\gitlab\NDK_Test\app>javah -d src\main\cpp\utils\dns -cp build\tmp\kotlin-classes\debug com.example.myapplication.utils.DnsUtils
```