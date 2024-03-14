#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
#include <time.h>
#include <android/log.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <android/asset_manager_jni.h>
//include <android/asset_manager.h>

#include "public.h"
#include "zipperdown.h"

char szCmdline[PATH_MAX] = {0};

void __attribute__ ((constructor)) SO_Load(){
	writeLogFile("test SO_Load\n");
	__android_log_print(ANDROID_LOG_ERROR,"SO_Load","SO_Load");
	printf("SO_Load\r\n");
	return;
}


void __attribute__ ((destructor)) SO_Unload(){
	writeLogFile("test SO_Unload\n");
	__android_log_print(ANDROID_LOG_ERROR,"SO_Unload","SO_Unload");
	printf("SO_Unload\r\n");
	return;
}



extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* javavm, void* reserved){
	int ret = 0;
	__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","test");
	writeLogFile("zipperdown so run\n");

	JNIEnv * env = NULL;
	bool bAttached = false;
	if (javavm == NULL){
		__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JavaVM NULL");
		return -1;
	}else{
		int ret = javavm->GetEnv((void**) &env, JNI_VERSION_1_4);
		if (ret < 0){
			__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JNIEnv.GetEnv error");
			ret = javavm->AttachCurrentThread(&env, NULL);
			if (ret < 0)
			{
				__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JavaVM.AttachCurrentThread error");
				return -1;
			}else{
				bAttached = true;
				__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JavaVM.AttachCurrentThread ok");
			}
		}else{
			__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JNIEnv.GetEnv ok");
		}
	}

	//ret = GetCmdline(szCmdline);
	ret = GetPackageName(env,getGlobalContext(env),szCmdline,PATH_MAX);
	char szlog[PATH_MAX];
	sprintf(szlog,"package:%s lib:zipperdown running\r\n",szCmdline);
	writeLogFile(szlog);



    if(bAttached == true){
    	javavm->DetachCurrentThread();
    	__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JavaVM.DetachCurrentThread ok");
    }
    return JNI_VERSION_1_1;
}
