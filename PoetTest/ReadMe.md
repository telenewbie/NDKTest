## 项目介绍
1. 测试注解的使用，以及 javapoet 的使用

    1. 需要再 `src/main/` 底下增加`resources/META-INF/services` 新建文件 `javax.annotation.processing.Processor`
    2. 需要再继承自javax.annotation.processing.AbstractProcessor的子类中 增加 ``getSupportedAnnotationTypes``返回支持的注解类型 才可以被调用


https://www.jianshu.com/p/ac6a69adecff

```
警告: No SupportedSourceVersion annotation found on com.example.poettest.HelloAnnotationProcessorJava, returning RELEASE_6.
警告: 来自注释处理程序 'org.gradle.api.internal.tasks.compile.processing.TimeTrackingProcessor' 的受支持 source 版本 'RELEASE_6' 低于 -source '1.8'
警告: No SupportedAnnotationTypes annotation found on com.example.poettest.HelloAnnotationProcessorJava, returning an empty set.
```
按照上面的写法会有这三个警告，而且还不会调用process 这个方法，所以需要增加



```Kotlin
    override fun getSupportedSourceVersion(): SourceVersion {
        return SourceVersion.RELEASE_8
    }

    override fun getSupportedAnnotationTypes(): Set<String> {
        val types = LinkedHashSet<String>()
        types.add(HelloClass::class.java.canonicalName)
        return types
    }
```