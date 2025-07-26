
#include "myapp.h"

extern "C" {
JNIEXPORT jstring JNICALL
Java_example_myapp_MainActivity_helloworld(JNIEnv *env, jobject obj) {
    return env->NewStringUTF("NATIVE LIB CODE!");
}
}
