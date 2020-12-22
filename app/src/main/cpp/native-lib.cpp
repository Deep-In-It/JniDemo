#include <jni.h>
#include <string>
#include <android/log.h>
#include <iostream>

#define TAG "native-lib_Log"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mysterycode_myjni_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_mysterycode_myjni_MainActivity_test1(JNIEnv *env, jobject thiz, jboolean b, jbyte b1,
                                              jchar c, jshort s, jlong l, jfloat f, jdouble d,
                                              jstring name, jint age, jintArray i,
                                              jobjectArray strs, jobject person,
                                              jbooleanArray barray) {
    unsigned char b_boolean = b;
    LOGD("boolean -> %d", b_boolean);
    char c_byte = b1;
    unsigned short c_char = c;
    short s_short = s;
    long l_long = l;
    float f_float = f;
    double d_double = d;
    const char *name_string = env->GetStringUTFChars(name, 0);
    int age_java = age;
    jint *intArray = env->GetIntArrayElements(i, NULL);
    jsize intArraySize = env->GetArrayLength(i);
    for (int i = 0; i < intArraySize; ++i) {
        LOGD("intArray->%d", intArray[i])
    }
    env->ReleaseIntArrayElements(i, intArray, 0);

    jsize stringArrayLength = env->GetArrayLength(strs);
    for (int i = 0; i < stringArrayLength; ++i) {
        jobject jobject1 = env->GetObjectArrayElement(strs, i);
        //强转 JNI String
        jstring stringArrayData = static_cast<jstring>(jobject1);
        //转 C  String
        const char *itemStr = env->GetStringUTFChars(stringArrayData, NULL);
        LOGD("String[%d]:%s", i, itemStr);
        //回收 String[]
        env->ReleaseStringUTFChars(stringArrayData, itemStr);
    }

    const char *person_class_str = "com/mysterycode/myjni/Person";
    jclass person_class = env->FindClass(person_class_str);
    const char *sig = "()Ljava/lang/String;";
    jmethodID jmethodId = env->GetMethodID(person_class, "getName", sig);
    jobject obj_string = env->CallObjectMethod(person, jmethodId);
    jstring perStr = static_cast<jstring>(obj_string);
    const char *itemStr2 = env->GetStringUTFChars(perStr, NULL);
    LOGD("Person:%s", itemStr2);
    env->DeleteLocalRef(person_class);
    env->DeleteLocalRef(person);
    jsize booArrayLength = env->GetArrayLength(barray);
    jboolean *bArray = env->GetBooleanArrayElements(barray, NULL);
    for (int i = 0; i < booArrayLength; ++i) {
        bool b = bArray[i];
        jboolean b2 = bArray[i];
        LOGD("boolean:%d", b);
        LOGD("jboolean:%d", b2);

    }
    env->ReleaseBooleanArrayElements(barray, bArray, 0);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_mysterycode_myjni_MainActivity_getPerson(JNIEnv *env, jobject thiz) {
    const char *clsName = "com/mysterycode/myjni/Person";
    jclass cls = env->FindClass(clsName);
    const char *method = "<init>";//Java构造方法的表示
    jmethodID constructId = env->GetMethodID(cls, method, "()V");
    jobject person_obj = env->NewObject(cls, constructId);

    const char *setSig = "(Ljava/lang/String;)V";
    jmethodID setName = env->GetMethodID(cls, "setName", setSig);

    const char *ageSig = "(I)V";
    jmethodID setAge = env->GetMethodID(cls, "setAge", ageSig);

    const char *name = "Dev";
    jstring newStringName = env->NewStringUTF(name);
    env->CallVoidMethod(person_obj, setName, newStringName);
    env->CallVoidMethod(person_obj, setAge, 1);

    const char *sig = "()Ljava/lang/String;";
    jmethodID toString = env->GetMethodID(cls, "toString", sig);
    jobject obj_string = env->CallObjectMethod(person_obj, toString);
    jstring pStr = static_cast<jstring>(obj_string);
    const char *itemStr2 = env->GetStringUTFChars(pStr, NULL);
    LOGD("Person:%s", itemStr2);
    return person_obj;
}


const char *classPathName = "com/mysterycode/myjni/DynamicJniActivity";

extern "C"
JNIEXPORT void JNICALL
native_dynamicRegister(JNIEnv *env, jobject thiz,
                       jstring name) {
    const char *j_name = env->GetStringUTFChars(name, NULL);
    LOGD("动态注册：%s", j_name);
    env->ReleaseStringUTFChars(name, j_name);
}

static const JNINativeMethod jniNativeMethod[] = {
        {"dynamicRegister", "(Ljava/lang/String;)V", (void *) (native_dynamicRegister)}
};

extern "C"
JNIEXPORT void JNICALL
native_dynamicRegister2(JNIEnv *env, jobject thiz, jstring name) {
    const char *j_name = env->GetStringUTFChars(name,NULL);
    LOGD("动态：%s",j_name);
    jclass cls = env->GetObjectClass(thiz);
    jmethodID jmethodId=env->GetMethodID(cls,"testException","()V");
    env->CallVoidMethod(thiz,jmethodId);
    //如果Java中发生异常后，此处捕捉处理
    jthrowable exc=env->ExceptionOccurred();
    if (exc){
        env->ExceptionDescribe();
        env->ExceptionClear();
        //抛出Jni的异常
        jclass newExcCls = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(newExcCls,"Jni 中发生了一个异常");
    }
    env->ReleaseStringUTFChars(name,j_name);
}

static const JNINativeMethod jniNativeMethod2[] = {
        {"dynamicRegister2", "(Ljava/lang/String;)V", (void *) (native_dynamicRegister2)}
};

/**
 * 该函数定义在jni.h头文件中，System.loadLibrary()时会调用JNI_OnLoad()函数
 */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *javaVm, void *pVoid) {
    //通过虚拟机 创建全新的 env
    JNIEnv *jniEnv = nullptr;
    jint result = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
    if (result != JNI_OK) {
        return JNI_ERR;
    }
    jclass dyactivity = jniEnv->FindClass(classPathName);
    jniEnv->RegisterNatives(dyactivity, jniNativeMethod,
                            sizeof(jniNativeMethod) / sizeof(JNINativeMethod));
    jniEnv->RegisterNatives(dyactivity, jniNativeMethod2,
                            sizeof(jniNativeMethod2) / sizeof(JNINativeMethod));
    return JNI_VERSION_1_6;
}



