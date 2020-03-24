package com.example.annotationtwo

import javax.annotation.processing.*
import javax.lang.model.SourceVersion
import javax.lang.model.element.TypeElement
import javax.tools.Diagnostic

@SupportedAnnotationTypes("com.example.annotationtwo.Two")
@SupportedSourceVersion(SourceVersion.RELEASE_7)
@SupportedOptions("myoption")
class TwoProcessor2 : AbstractProcessor() {
    var messager: Messager? = null

    override fun init(processingEnv: ProcessingEnvironment) {
        super.init(processingEnv)
        messager = processingEnv.messager
        messager?.printMessage(Diagnostic.Kind.WARNING, "TwoProcessor2:init")

        val myoption = processingEnv.options["myoption"]
        messager?.printMessage(Diagnostic.Kind.WARNING, "TwoProcessor2:$myoption")
    }

    override fun process(annotations: Set<TypeElement>, roundEnv: RoundEnvironment): Boolean {
        messager!!.printMessage(Diagnostic.Kind.WARNING, "TwoProcessor2 process")
        return false
    }
}