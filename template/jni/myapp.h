#ifndef MY_APP_H
#define MY_APP_H 

#include <jni.h>

extern "C" {
   JNIEXPORT jstring JNICALL
   Java_example_myapp_MainActivity_helloworld(JNIEnv *env, jobject obj);
}


#endif /* ifndef MY_APP_H */
