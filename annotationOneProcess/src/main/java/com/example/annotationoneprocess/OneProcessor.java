package com.example.annotationoneprocess;

import com.example.annotationone.One;

import java.util.HashSet;
import java.util.Set;

import javax.annotation.processing.AbstractProcessor;
import javax.annotation.processing.Messager;
import javax.annotation.processing.ProcessingEnvironment;
import javax.annotation.processing.RoundEnvironment;
import javax.lang.model.SourceVersion;
import javax.lang.model.element.TypeElement;
import javax.tools.Diagnostic;

public class OneProcessor extends AbstractProcessor {
    private Messager mMessager;

    @Override
    public synchronized void init(ProcessingEnvironment processingEnv) {
        super.init(processingEnv);
        mMessager = processingEnv.getMessager();
        // annotationProcessor  支持以下四种的打印，但是kapt 只支持 以下2种
        mMessager.printMessage(Diagnostic.Kind.MANDATORY_WARNING, "OneProcessor init 1");
        mMessager.printMessage(Diagnostic.Kind.WARNING, "OneProcessor init 2");
        //kapt 不支持以下两种的打印
        mMessager.printMessage(Diagnostic.Kind.NOTE, "OneProcessor init 3");
        mMessager.printMessage(Diagnostic.Kind.OTHER, "OneProcessor init 4");
    }

    @Override
    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
        mMessager.printMessage(Diagnostic.Kind.WARNING, "OneProcessor process");
        mMessager.printMessage(Diagnostic.Kind.WARNING, "OneProcessor process");
        mMessager.printMessage(Diagnostic.Kind.WARNING, "OneProcessor process");
        mMessager.printMessage(Diagnostic.Kind.WARNING, "OneProcessor process");
        return false;
    }

    @Override
    public Set<String> getSupportedAnnotationTypes() {
        Set<String> sets = new HashSet<>();
        sets.add(One.class.getCanonicalName());
        return sets;
    }

    @Override
    public SourceVersion getSupportedSourceVersion() {
        return SourceVersion.RELEASE_8;
    }
}
