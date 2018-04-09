# AndroidStudio jni开发环境搭建和常见错误解决
1、下载ndk
ndk可以直接通过sudio下载(Tools->Android->SDK Manager->SDK tools里下载cmake和ndk)
或者直接到[AndroidDevTools](http://www.androiddevtools.cn/ "androiddevtools")下载，像配置sdk一样配置ndk的环境。

2、新建Android项目时勾选Include C++ Support，估计也是android以后主推的方式，我是通过Androidstudio下载最新的ndk编译时出现下面的错误

    * What went wrong:
	A problem occurred configuring project ':app'.
	> executing external native build for cmake F:\github\JniTest\app\CMakeLists.txt
通过查看Gralde Console输出信息发现

    Configuration on demand is an incubating feature.
	CMake Error at C:/Users/xxx/AppData/Local/Android/Sdk/ndk-bundle/build/cmake/android.toolchain.cmake:312 (message):
 	 Invalid Android ABI: mips64.  (MIPS and MIPS64 are no longer supported.)
	Call Stack (most recent call first):
 	 C:/Users/xxx/AppData/Local/Android/Sdk/cmake/3.6.4111459/share/cmake-3.6/Modules/CMakeDetermineSystem.cmake:98 (include)
 	 CMakeLists.txt
	CMake Error: CMAKE_C_COMPILER not set, after EnableLanguage
	CMake Error: CMAKE_CXX_COMPILER not set, after EnableLanguage
	-- Configuring incomplete, errors occurred!

	FAILURE: Build failed with an exception.
  然来是下载的最新ndk不支持 mips64、mips、armeabi，如果就是需要编译这些so，可下载低版本的ndk尝试，如果不需要这些版本的so，可以在build.gradle中指定需要的so类型，在defaultConfig节点下配置如下：
  
  
    android {
		compileSdkVersion 26
		buildToolsVersion "26.0.0"
		defaultConfig {
		    applicationId "com.jason.jnitest"
		    minSdkVersion 21
		    targetSdkVersion 26
		    versionCode 1
		    versionName "1.0"
		    testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"
		    externalNativeBuild {
		        cmake {
		            cppFlags ""
		        }
		    }
		    ndk {
		       abiFilters "armeabi-v7a", "x86"   //配置需要的so
		    }
		}
   	}
   这样就可以解决编译so出错的问题，如果有其它编译错误，可根据Gralde Console输出信息查看具体build failed原因，另外需要注意的是，在.cpp中实现对应函数时候，需要在函数前面加上extern "C"，不然会出现java.lang.UnsatisfiedLinkError: No implementation found for java.lang.String 的错误
   
   	#include <jni.h>
	#include <string>
	#include "android/log.h"
	
	static const char *TAG = "jason";
	#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
	
	extern "C" 
	JNIEXPORT void JNICALL
	Java_com_jason_jni_demo_MainActivity_sayHelloToC(JNIEnv *env, jclass type, jstring str_) {
	    const char *str = env->GetStringUTFChars(str_, 0);
	
	    // TODO
	    LOGE("str :%s ", str);
	    env->ReleaseStringUTFChars(str_, str);
	}
	
	
	//java.lang.UnsatisfiedLinkError: No implementation found for java.lang.String com.jason.jni.demo.MainActivity.stringFromJNI(),注释下面这行会出现这个错误
	extern "C" //没有此行会抛出上面的错误
	JNIEXPORT jstring JNICALL
	Java_com_jason_jni_demo_MainActivity_stringFromJNI(
	        JNIEnv *env,
	        jobject /* this */) {
	    std::string hello = "Hello from C++";
	    return env->NewStringUTF(hello.c_str());
	}
  或者按照下面的格式：
  
  	#include <jni.h>
	#include <string>
	#include "android/log.h"
	
	static const char *TAG = "jason";
	#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
	
	extern "C" { //实现的函数放到此标签内
		JNIEXPORT void JNICALL
		Java_com_jason_jni_demo_MainActivity_sayHelloToC(JNIEnv *env, jclass type, jstring str_) {
		    const char *str = env->GetStringUTFChars(str_, 0);
		
		    // TODO
		    LOGE("str :%s ", str);
		    env->ReleaseStringUTFChars(str_, str);
		}
		
		
		//java.lang.UnsatisfiedLinkError: No implementation found for java.lang.String com.jason.jni.demo.MainActivity.stringFromJNI(),注释下面这行会出现这个错误
		JNIEXPORT jstring JNICALL
		Java_com_jason_jni_demo_MainActivity_stringFromJNI(
		        JNIEnv *env,
		        jobject /* this */) {
		    std::string hello = "Hello from C++";
		    return env->NewStringUTF(hello.c_str());
		}
	}
