package com.example.annotationtwo

import javax.annotation.processing.*
import javax.lang.model.SourceVersion
import javax.lang.model.element.TypeElement
import javax.tools.Diagnostic

@SupportedAnnotationTypes("com.example.annotationtwo.Two"/*, "kotlin.Metadata"*/)
@SupportedSourceVersion(SourceVersion.RELEASE_7)
class TwoProcessor2 : AbstractProcessor() {
    var messager: Messager? = null

    override fun init(processingEnv: ProcessingEnvironment) {
        super.init(processingEnv)
        messager = processingEnv.messager
        messager?.printMessage(Diagnostic.Kind.WARNING, "newbie:init")
    }

//    override fun process(annotations: Set<TypeElement>, roundEnv: RoundEnvironment): Boolean {
//        messager!!.printMessage(Diagnostic.Kind.ERROR, "TwoProcessor2 process")
//        return false
//    }

    //  这里！！！！！！！！！！！！！！！！！！！
    override fun process(
        annotations: MutableSet<out TypeElement>,
        roundEnv: RoundEnvironment
    ): Boolean {
        messager?.printMessage(Diagnostic.Kind.WARNING, "newbie:process")
        return true
    }

}