#include <jni.h>
#include <string>
#include "android/log.h"

static const char *TAG = "jason";
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

extern "C" {
JNIEXPORT void JNICALL
Java_com_jason_jni_demo_MainActivity_sayHelloToC(JNIEnv *env, jclass type, jstring str_) {
    const char *str = env->GetStringUTFChars(str_, 0);

    // TODO
    LOGE("str :%s ", str);
    env->ReleaseStringUTFChars(str_, str);
}


//java.lang.UnsatisfiedLinkError: No implementation found for java.lang.String com.jason.jni.demo.MainActivity.stringFromJNI(),注释下面这行会出现这个错误
//extern "C"
JNIEXPORT jstring JNICALL
Java_com_jason_jni_demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
}