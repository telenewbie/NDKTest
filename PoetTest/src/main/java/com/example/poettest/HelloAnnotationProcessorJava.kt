//package com.example.poettest
//
//import javax.annotation.processing.AbstractProcessor
//import javax.annotation.processing.Messager
//import javax.annotation.processing.ProcessingEnvironment
//import javax.annotation.processing.RoundEnvironment
//import javax.lang.model.SourceVersion
//import javax.lang.model.element.Element
//import javax.lang.model.element.ElementKind
//import javax.lang.model.element.TypeElement
//import javax.tools.Diagnostic
//import javax.tools.StandardLocation
//
//class HelloAnnotationProcessorJava : AbstractProcessor() {
//    private var mMessager: Messager? = null
//    override fun init(processingEnv: ProcessingEnvironment?) {
//        super.init(processingEnv)
//        mMessager = processingEnv?.messager
//        mMessager!!.printMessage(Diagnostic.Kind.NOTE, "printMessage : init")
//    }
//
//    override fun process(annotations: Set<TypeElement>, roundEnv: RoundEnvironment): Boolean {
//
//        roundEnv.getElementsAnnotatedWith(HelloClass::class.java)?.forEach { element ->
//            val packageName = processingEnv.elementUtils.getPackageOf(element).qualifiedName.toString()
//            val simpleName = element.simpleName.toString()
//
////            processingEnv.filer.createResource(StandardLocation.SOURCE_OUTPUT, packageName, "${simpleName}Generated.kt", element)
////                    .openWriter().use {
////                        it.write(
////                                """
////                    package $packageName
////                    class ${simpleName}Generated
////                """.trimIndent()
////                        )
////                    }
//            // 使用javapoet 进行生成代码
//
//
////            element.kind.isInterface
//            // 自动生成代码
//
//        }
//        processingEnv.messager.printMessage(Diagnostic.Kind.NOTE, "process over")
//        return false
//    }
//
//    override fun getSupportedSourceVersion(): SourceVersion = SourceVersion.RELEASE_8
//
//    override fun getSupportedAnnotationTypes(): Set<String> {
//        return setOf(HelloClass::class.java.canonicalName)
//    }
//}
