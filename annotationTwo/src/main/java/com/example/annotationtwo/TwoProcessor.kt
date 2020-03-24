package com.example.annotationtwo

import javax.annotation.processing.*
import javax.lang.model.SourceVersion
import javax.lang.model.element.TypeElement
import javax.tools.Diagnostic

@SupportedAnnotationTypes("com.example.annotationtwo.Two"/*, "kotlin.Metadata"*/)
@SupportedSourceVersion(SourceVersion.RELEASE_7)
class TwoProcessor : AbstractProcessor() {
    var mMessager: Messager? = null

    override fun init(processingEnv: ProcessingEnvironment) {
        super.init(processingEnv)
        mMessager = processingEnv.messager
        mMessager?.printMessage(Diagnostic.Kind.WARNING, "TwoProcessor init ")
    }

    override fun process(
            annotations: MutableSet<out TypeElement>,
            roundEnv: RoundEnvironment
    ): Boolean {
        mMessager?.printMessage(Diagnostic.Kind.WARNING, "TwoProcessor process")
        return true
    }
}