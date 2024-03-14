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
#include <sys/file.h>
#include <dirent.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <elf.h>
#include <sys/system_properties.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include "public.h"
#include "qqckey.h"
#include "qqnews.h"
#include "TencentMedia.h"
#include "stormPlayer.h"
#include "toutiao.h"
#include "weixin.h"
#include "neteasenews.h"
#include "iqiyi.h"
#include "alicdn.h"
#include "youku.h"
#include "douyu.h"
#include "Jni_wgs2gcj.h"
#include "p2p.h"

using namespace std;



JNIEnv * gEnv = 0;
JavaVM* gJvm = 0;

soinfo gKeepSoInfo = {0};


//我们需要了解gcc新引进的选项-fvisibility=hidden，这个编译选项可以把所有的符号名（包括函数名和全局变量名）都强制标记成隐藏属性。
//我们可以在Android.mk中可以通过修改LOCAL_CFLAGS选项加入-fvisibility=hidden来做到这一点
//源代码里出现的函数名和全局变量名（符号名）都变成了't'，也就是说都是局部符号（类似于static）
//void__attribute__ ((visibility ("default")))Java_com_example_SanAngeles_DemoRenderer_nativeInit ( JNIEnv*  env )

extern "C" string JniLoadJar(JNIEnv * env,const char * szCmdline){
	int ret = 0;
	char szout[PATH_MAX] = {0};
	char workpath[PATH_MAX] = {0};

//	ret = remove(LOG_FILE_NAME);
//	char szlogfn[PATH_MAX] = {0};
//	sprintf(szlogfn,"/sdcard/%s_running.log",szCmdline);
//	ret = remove(szlogfn);

	if(strstr(szCmdline,"com.tencent.mm") ){
		ret = weixinSoProcess(workpath,szCmdline,&gKeepSoInfo);
	}else if( strstr(szCmdline,"com.tencent.mobileqq")  || strstr(szCmdline,"com.tencent.tim") ){
		ret = qqckeyProcess(workpath,szCmdline);
	}else if(strstr(szCmdline,"com.tencent.news") ){
		ret = qqnewsProcess(workpath,szCmdline);
	}else if(strstr(szCmdline,"com.qiyi.video") ){
		ret = IqiyiVideo::iqiyiProcess(workpath,szCmdline);
	}
	else if(strstr(szCmdline,"com.ss.android.article.news") ){
		ret = toutiaoProcess(workpath,szCmdline);
	}else if(strstr(szCmdline,"com.storm.smart") ){
		ret = stormplayerProcess(workpath,szCmdline);
	}else if(strstr(szCmdline,"com.tencent.qqlive") ){
		ret = qqliveProcess(workpath,szCmdline);
		if(ret < 0){
			ret = tencentMidiaProcess(workpath,szCmdline);
		}
	}else if(strstr(szCmdline,"com.tencent.qqmusic") ){
		ret = tencentMidiaProcess(workpath,szCmdline);
	}else if(strstr(szCmdline,"com.netease.newsreader.activity")){
		ret = NeteaseNews::neteasenewsProcess(workpath,szCmdline);
	}
	else if(strstr(szCmdline,"com.tencent.FileManager") ){
		androidCopyFile((char*)"/data/data/com.tencent.FileManager/lib/libqb_keystore.so",
				(char*)"/data/data/com.tencent.FileManager/files/libqb_keystore.so");
	    strcpy(workpath,"/data/data/");
	    strcat(workpath,szCmdline);
	    strcat(workpath,"/files/");
	    ret = splitFromSo(workpath,workpath,(char*)"libqb_keystore.so");
	}else if( strstr(szCmdline,"com.tencent.") ){
		ret = qqckeyProcess(workpath,szCmdline);
	}else if( strstr(szCmdline,"com.ximalaya.ting.android") ){
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","com.ximalaya.ting.android");
		strcpy(workpath,"/data/data/com.ximalaya.ting.android/app_dex/reactnative/origin/lib/");
	}else if(strstr(szCmdline,"com.youku.phone")){
		ret = Youku::youkuProc(workpath,szCmdline);
	}
	else if(strstr(szCmdline,"com.UCMobile")){
			ret = Youku::youkuProc(workpath,szCmdline);
	}
	else if(strstr(szCmdline,"air.tv.douyu.android")){
			ret = Douyu::douyuProcess(workpath,szCmdline);
	}
	else if(strstr(szCmdline,"com.kugou.android") || strstr(szCmdline,"cn.kuwo.player") ||
			strstr(szCmdline,"com.changba")){
			ret = wgs2gcj_initProcess(workpath,szCmdline);
	}
	else{
		ret = AliCdn::alicdnProc(workpath,szCmdline);
		//writeLogFile("not recognized package:%s\r\n",szCmdline);
		//return -1;
	}

    if(ret < 0){
    	writeLogFile("not found so path or apk path\r\n");
    	return "";
    }else{
    	sprintf(szout,"find work path:%s\r\n",workpath);
    	writeLogFile(szout,szCmdline);
    }

	char defjarpath[PATH_MAX];
	strcpy(defjarpath,workpath);
	strcat(defjarpath,JAR_FILE_NAME);

	char optjarpath[PATH_MAX];
	strcpy(optjarpath,"/data/data/");
	strcat(optjarpath,szCmdline);
	strcat(optjarpath,"/cache/");
	if(access(defjarpath,F_OK) != 0){
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","access error:%s",defjarpath);
		return "";
	}else{
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","defaultjarpath:%s,optjarpath:%s",defjarpath,optjarpath);
	}

	jstring jstrdefjarpath = char2Jstring(env,defjarpath,strlen(defjarpath));
	jstring jstroptjarpath = char2Jstring(env,optjarpath,strlen(optjarpath));
	jstring jstrclassname = char2Jstring(env,ENTRANCE_CLASSNAME,strlen(ENTRANCE_CLASSNAME));
	jstring jstrmethodname = char2Jstring(env,ENTRANCE_METHODNAME,strlen(ENTRANCE_METHODNAME));

	jclass classloader = env->FindClass("java/lang/ClassLoader");
	jmethodID getsysclassloader = env->GetStaticMethodID(classloader, "getSystemClassLoader","()Ljava/lang/ClassLoader;");
	jobject loader = env->CallStaticObjectMethod(classloader,getsysclassloader);

	jclass dexclassloader = env->FindClass("dalvik/system/DexClassLoader");
	jmethodID dexclsldinit = env->GetMethodID(dexclassloader,"<init>",
			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
	jobject dexloader =env->NewObject(dexclassloader,dexclsldinit, jstrdefjarpath, jstroptjarpath, 0, loader);

	jclass dexloaderclass = env->GetObjectClass(dexloader);

	jmethodID findclass = env->GetMethodID(dexloaderclass,"loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	if(NULL==findclass){
		findclass = env->GetMethodID(dexloaderclass,"findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	}

	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","ClassLoader:%x,getSystemClassLoader:%x,loader:%x,"
			"DexClassLoader:%x,DexClassLoader init:%x,DexClassLoader class:%x,dexloaderclass:%x,findClass:%x",
			(unsigned int)classloader,(unsigned int)getsysclassloader,(unsigned int)loader,
			(unsigned int)dexclassloader,(unsigned int)dexclsldinit,(unsigned int)dexloader,
			(unsigned int)dexloaderclass,(unsigned int)findclass);

	jclass javaenterclass=(jclass)env->CallObjectMethod(dexloader,findclass,jstrclassname);
	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","%s:%x",ENTRANCE_CLASSNAME,(unsigned int)javaenterclass);

	jmethodID enterclassinit = env->GetMethodID(javaenterclass, "<init>", "()V");
	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","enterclassinit:%x",(unsigned int)enterclassinit);

	jobject enterclassobj = env->NewObject(javaenterclass,enterclassinit);
	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","enterclassobj:%x",(unsigned int)enterclassobj);


	//jmethodID entermethodid = env->GetMethodID(javaenterclass, ENTRANCE_METHODNAME, "(Landroid/content/Context;)V");
	jmethodID entermethodid = env->GetMethodID(javaenterclass, ENTRANCE_METHODNAME,
			"(Landroid/content/Context;Ljava/lang/String;)V");
	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","entermethodid:%x",(unsigned int)entermethodid);
	if(entermethodid){
		jclass atclass = env->FindClass("android/app/ActivityThread");
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","atclass:%x",(unsigned int)atclass);

		jmethodID catmid = env->GetStaticMethodID(atclass,"currentActivityThread","()Landroid/app/ActivityThread;");
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","catmid:%x",(unsigned int)catmid);

		jobject catobj = env->CallStaticObjectMethod(atclass,catmid);
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","catobj:%x",(unsigned int)catobj);

		jmethodID getappmid = env->GetMethodID(atclass, "getApplication", "()Landroid/app/Application;");
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","getappmid:%x",(unsigned int)getappmid);

		jobject contextobj = env->CallObjectMethod(catobj, getappmid);
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","contextobj:%x",(unsigned int)contextobj);

		//env->CallVoidMethod(enterclassobj, entermethodid, contextobj);

		jstring jstrworkpath = char2Jstring(env,workpath,strlen(workpath));
		env->CallVoidMethod(enterclassobj, entermethodid, contextobj,jstrworkpath);

		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","jar or dex entering");

		writeLogFile("so and jar loaded ok\r\n",szCmdline);


		return workpath;
	}else{
		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","entermethodid not found");
	}

	return "";
}


//dlopen do not call JNI_OnLoad,but System.load or System.loadLibrary will do
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* javavm, void* reserved){

	int jniver = DEFAULT_JNI_VERSION;

	__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","so start");

	char szlog[PATH_MAX];
	int ret = 0;
	JNIEnv *env = NULL;
	bool bAttached = false;
	if (javavm == NULL){
		__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JavaVM NULL");
		return jniver;
	}else{
		gJvm = javavm;
		ret = javavm->GetEnv((void**) &env, DEFAULT_JNI_VERSION);
		if (ret < 0){
			__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JNIEnv.GetEnv error");

			ret = javavm->AttachCurrentThread(&env, NULL);
			if (ret < 0)
			{
				__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JavaVM.AttachCurrentThread error");
				return jniver;
			}else{
				bAttached = true;
				gEnv = env;
				__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JavaVM.AttachCurrentThread ok");
			}
		}else{
			gEnv = env;
			__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JNIEnv.GetEnv ok");
		}
	}

	char szCmdline[PATH_MAX] = {0};
	int fs = 0;
	char * data = 0;
	ret = readFile4096((char*)"/proc/self/cmdline",&data,&fs);
	if(ret == 0){
		ret = getPacknameInProcname(data,szCmdline);
		sprintf(szlog,"JNI_OnLoad get package:%s,process:%s\r\n",szCmdline,data);
		writeLogFile(szlog,szCmdline);
		delete []data;
	}else{
		jobject obj = getGlobalContext(env);
		if(obj > 0){
			ret = GetPackageName(env,obj,szCmdline,PATH_MAX);
			sprintf(szlog,"JNI_OnLoad get package:%s\r\n",szCmdline);
			writeLogFile(szlog,szCmdline);
		}else{
			writeLogFile("JNI_OnLoad not found context\r\n",szCmdline);
		}
	}

	string workpath = JniLoadJar(env,szCmdline);
	if(workpath != ""){
		__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","get workpath:%s",workpath.c_str());

		if(strstr(szCmdline,"air.tv.douyu.android") ||strstr(szCmdline,"com.tencent.news")){
			string oldsopath = string(workpath) + OLDSO_FILE_NAME;
			void * handle = (void*)dlopen(oldsopath.c_str(),RTLD_LAZY|RTLD_GLOBAL);
			if(handle ){
				__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","dlopen:%x",handle);
				typedef int (*ptrJniOnLoad)(JavaVM* javavm, void* reserved);
				ptrJniOnLoad lpJniOnLoad = (ptrJniOnLoad)dlsym(handle,"JNI_OnLoad");
				if(lpJniOnLoad){
					jniver = lpJniOnLoad(javavm,reserved);
					__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","dlsym JNI_OnLoad:%x",lpJniOnLoad);
				}else{
					sprintf(szlog,"not found JNI_OnLoad in old so:%s\r\n",oldsopath.c_str());
					writeLogFile(szlog,szCmdline);
				}
			}else{
				sprintf(szlog,"dlopen so:%s error\r\n",oldsopath.c_str());
				writeLogFile(szlog,szCmdline);
			}
		}
	}


    if(bAttached == true){
    	javavm->DetachCurrentThread();
    	__android_log_print(ANDROID_LOG_ERROR,"JNI_OnLoad","JavaVM.DetachCurrentThread ok");
    }
    return jniver;
}


extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* javavm, void* reserved){

	__android_log_print(ANDROID_LOG_ERROR,"JNI_OnUnload","JNI_OnUnload");
	return;
}


//System.loadLibrary call dlopen->init or initarray,call JNI_OnLoad everytime
void __attribute__ ((constructor)) SO_Load(void){

	__android_log_print(ANDROID_LOG_ERROR,"SO_Load","SO_Load");
	printf("SO_Load\r\n");

	int ver = getAndroidVersion();
	if(ver >= 24){
		return;
	}else{
		char szlog[1024];
		JNIEnv * env = dlsmgetenv();
		sprintf(szlog,"android version:%u,dlsmgetenv get jvm env:%p\r\n",ver,env);
		writeLogFile(szlog);
		if(env == 0){
			return;
		}else{
			JavaVM * jvm = 0;
			int ret = env->GetJavaVM(&jvm);
			sprintf(szlog,"get java vm:%p ret:%d\r\n",jvm,ret);
			writeLogFile(szlog);
			if(ret < 0){
				return;
			}else{
				ret = JNI_OnLoad(jvm,0);
				//sprintf(szlog,"JNI_OnLoad ret value:%u\r\n",ret);
				//writeLogFile(szlog);
			}
		}
	}

//	char szCmdline[1024];
//	int fs = 0;
//	char * data = 0;
//	int ret = readFile4096((char*)"/proc/self/cmdline",&data,&fs);
//	if(ret == 0){
//		sprintf(szlog,"SO_Load get package name:%s\r\n",data);
//		writeLogFile(szlog,data);
//
//		getPackageNameFromProcessName(data);
//		strcpy(szCmdline,data);
//		delete []data;

//		//if(strstr(szCmdline,"com.tencent.mm")){
//			char workpath[1024] = {0};
//			soinfo gKeepSoInfo = {0};
//			int ret = weixinSoProcess(workpath,"com.tencent.mm",&gKeepSoInfo);
//			printf("SO_Load get workpath:%s,weixinSoProcess ret value:%u\r\n",workpath,ret);
//		//}
//	}else{
//		writeLogFile("SO_Load not found package name\r\n",szCmdline);
//	}

	return;
}


void __attribute__ ((destructor)) SO_Unload(void){
	__android_log_print(ANDROID_LOG_ERROR,"SO_Unload","SO_Unload");
	printf("SO_Unload\r\n");
	return;
}
