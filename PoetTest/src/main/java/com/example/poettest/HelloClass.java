package com.example.poettest;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

// 放在类上面
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.SOURCE)
public @interface HelloClass {
    String value();
}
