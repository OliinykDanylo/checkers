#include <jni.h>
#include <iostream>
#include "Main.h"

JNIEXPORT void JNICALL Java_Main_sayHello
        (JNIEnv *enc) {
    std::cout<<"Hello from c++"<<std::endl;
}