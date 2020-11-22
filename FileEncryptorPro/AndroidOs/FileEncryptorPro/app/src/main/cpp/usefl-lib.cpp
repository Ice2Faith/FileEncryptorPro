#include <jni.h>
#include <string>
#include <stdio.h>
#include<android/log.h>

void CppLogErr(const char * text,const char * tag="DebugInfo")
{
    __android_log_write(ANDROID_LOG_ERROR,tag,text);
}


#include "EveryFileLocker.hpp"



//extern "C" JNIEXPORT jstring JNICALL
//Java_com_ugex_savelar_fileencryptorpro_MainActivity_stringFromJNI(
//        JNIEnv *env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}


/**
 * 使用方式2进行注册，是有一些麻烦的，特别是注册的接口少的时候，尤其
 * 写函数
 * 写JNI_OnLoad
 * 填写注册数组
 */
static EveryFileLocker g_locker;

extern "C"
JNIEXPORT
jboolean JNICALL Java_com_ugex_savelar_fileencryptorpro_MainActivity_fileLock(JNIEnv* env,
                              jobject obj,
                              jstring _srcFileName,
                              jstring _drtFileName,
                              jstring _password)
{
    const char * srcFileName=env->GetStringUTFChars(_srcFileName,0);
    const char * drtFileName=env->GetStringUTFChars(_drtFileName,0);
    const char * password=env->GetStringUTFChars(_password,0);


    bool rst=g_locker.lock(srcFileName,drtFileName,password);
    jboolean ret=JNI_FALSE;
    if(rst)
        ret=JNI_TRUE;
    else
        ret=JNI_FALSE;
    return ret;
}

extern "C"
JNIEXPORT
jboolean JNICALL Java_com_ugex_savelar_fileencryptorpro_MainActivity_fileUnlock(JNIEnv* env,
                            jobject obj,
                            jstring _srcFileName,
                            jstring _drtFileName,
                            jstring _password)
{
    const char * srcFileName=env->GetStringUTFChars(_srcFileName,0);
    const char * drtFileName=env->GetStringUTFChars(_drtFileName,0);
    const char * password=env->GetStringUTFChars(_password,0);


    bool rst=g_locker.unlock(srcFileName,drtFileName,password);
    jboolean ret=JNI_FALSE;
    if(rst)
        ret=JNI_TRUE;
    else
        ret=JNI_FALSE;
    return ret;
}
