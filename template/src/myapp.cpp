#include <stdlib.h>
#include <jni.h>


JNIEXPORT jstring JNICALL
Java_example_myapp_MainActivity_helloworld(JNIEnv *env, jobject obj)
{
       return env->NewStringUTF("HELLO WORLD");
}
