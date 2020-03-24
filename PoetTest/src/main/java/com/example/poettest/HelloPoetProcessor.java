//package com.example.poettest;
//
//import java.io.IOException;
//import java.util.HashSet;
//import java.util.Set;
//import java.util.logging.Logger;
//
//import javax.annotation.processing.AbstractProcessor;
//import javax.annotation.processing.Filer;
//import javax.annotation.processing.Messager;
//import javax.annotation.processing.ProcessingEnvironment;
//import javax.annotation.processing.RoundEnvironment;
//import javax.lang.model.element.Element;
//import javax.lang.model.element.Modifier;
//import javax.lang.model.element.TypeElement;
//import javax.lang.model.util.Elements;
//import javax.tools.Diagnostic;
//
////import com.squareup.javapoet.ClassName;
////import com.squareup.javapoet.CodeBlock;
////import com.squareup.javapoet.JavaFile;
////import com.squareup.javapoet.MethodSpec;
////import com.squareup.javapoet.ParameterSpec;
////import com.squareup.javapoet.TypeName;
////import com.squareup.javapoet.TypeSpec;
//
//// 采用poet 自动生成代码
//public class HelloPoetProcessor extends AbstractProcessor {
//    private Messager mMessager;
//    private Elements mElementUtils;
//    private Filer mFiler;
//
//    @Override
//    public synchronized void init(ProcessingEnvironment processingEnv) {
//        super.init(processingEnv);
//        mMessager = processingEnv.getMessager();
//        mElementUtils = processingEnv.getElementUtils();
//        mFiler = processingEnv.getFiler();
//    }
//
//    @Override
//    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
//        Set<? extends Element> elementsAnnotatedWith = roundEnv.getElementsAnnotatedWith(HelloPoet.class);
//        for (Element clazz : elementsAnnotatedWith) {
//            HelloPoet annotation = clazz.getAnnotation(HelloPoet.class);
//            mMessager.printMessage(Diagnostic.Kind.ERROR, "newbie this " + annotation.debug());
//            generator(annotations, roundEnv, clazz);
//        }
//        mMessager.printMessage(Diagnostic.Kind.ERROR, "newbie over ");
//        // 采用 javapoet 自动生成文件
//
//
//        return false;
//    }
//
//    private void generator(Set<? extends TypeElement> annotations, RoundEnvironment processingEnv, Element clazz) {
//        mMessager.printMessage(Diagnostic.Kind.ERROR, "开始处理  " + clazz.getSimpleName());
//        String packageName = mElementUtils.getPackageOf(clazz).getQualifiedName().toString();
//        String className = clazz.getSimpleName().toString();
////
////        String proxyName = className + "_Newbie";
////        TypeSpec.Builder proxyBuilder = TypeSpec.classBuilder(proxyName);
////        //public class $proxyName {
////        //private boolean mIsProcess;
////        // private boolean isProcess(){return mIsProcess;}
////        // }
////        //
////        CodeBlock code = CodeBlock.builder().add(" private boolean isProcess(){return mIsProcess;}").build();
////        MethodSpec o = MethodSpec.constructorBuilder().addCode(code).build();
////
////        proxyBuilder.addModifiers(Modifier.PUBLIC).addField(TypeName.BOOLEAN, "mIsProcess", Modifier.PRIVATE).addMethod(o).build();
////
////        // 创建java文件
////        // 创建Java文件
////        try {
////            JavaFile proxyFile = JavaFile.builder(packageName, proxyBuilder.build()).build();
////            proxyFile.writeTo(mFiler);
////        } catch (IOException e) {
////            e.printStackTrace();
////        }
//    }
//
//    @Override
//    public Set<String> getSupportedAnnotationTypes() {
//        Set<String> support = new HashSet<>();
//        support.add(HelloPoet.class.getCanonicalName());
//        return support;
//    }
//}
