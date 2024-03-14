#include <jni.h>

extern "C" string JniLoadJar(JNIEnv * env,const char * szCmdline);

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* javavm, void* reserved);
