package com.example.annotationthree

import javax.annotation.processing.*
import javax.lang.model.SourceVersion
import javax.lang.model.element.TypeElement
import javax.tools.Diagnostic

@SupportedAnnotationTypes("com.example.annotationthree.Three")
@SupportedSourceVersion(SourceVersion.RELEASE_8)
class ThreeProcessor : AbstractProcessor() {
    private var mMessager: Messager? = null

    override fun init(processingEnv: ProcessingEnvironment) {
        super.init(processingEnv)
        mMessager = processingEnv.messager
        mMessager!!.printMessage(Diagnostic.Kind.ERROR, "ThreeProcessor init ")
    }

    override fun process(annotations: Set<TypeElement>, roundEnv: RoundEnvironment): Boolean {
        mMessager!!.printMessage(Diagnostic.Kind.ERROR, "ThreeProcessor process")
        return false
    }

}