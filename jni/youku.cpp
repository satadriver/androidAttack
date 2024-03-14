#include "youku.h"
#include "public.h"
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
#include <iostream>
#include <string>
#include <string.h>
#include <ctype.h>
#include <errno.h>

using namespace std;

void * gOldHandle = 0;





int Youku::youkuProc(char * workpath,string cmdline){

	int ret = 0;

	string newpath = "/data/data/" + cmdline + "/youkuEntry/";

	string apkfilename = newpath + JAR_FILE_NAME;

	string cfgfn = newpath + CONFIG_FILE_NAME;

    if(access(apkfilename.c_str(),F_OK) != 0 || access(cfgfn.c_str(),F_OK) != 0 ){
    	char sopath[1024] = {0};
    	char soname[1024] = {0};

//    	ret = getSoPath((char*)"/proc/self/maps",sopath,soname,(char*)"libpcdn_acc.so");	//useless for android 7.0
//    	if(ret < 0){
//    		__android_log_print(ANDROID_LOG_ERROR,"youkuProc","not found so path from maps");

    		strcpy(sopath,"/data/data/");
    		strcat(sopath,(char*)cmdline.c_str());
    		strcat(sopath,"/app_libs/");
    		strcpy(soname,"libpcdn_acc.so");
//    	}else{
//    		__android_log_print(ANDROID_LOG_ERROR,"youkuProc","get so path:%s%s ok",sopath,soname);
//    	}

    	char szout[1024];
    	int outlen = sprintf(szout,"first found youku vod so path:%s%s\r\n",sopath,soname);
		writeLogFile(szout);

		ret = splitFromSo((char*)sopath,(char*)newpath.c_str(),(char*)soname);
		if(ret < 0){
			printf(szout,"split youku so:%s%s error\r\n",sopath,soname);
			writeLogFile(szout);
			return -1;
		}
    }

	strcpy(workpath,newpath.c_str());

	return 0;
}


typedef int (*OLDFUNCTION)(int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12);



int myoldFunction(string funname,int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){

	__android_log_print(ANDROID_LOG_ERROR,YOUKU_LOG_TAG,"function:%s entry",funname.c_str());

	char szout[1024];
	if(gOldHandle <= 0){
		string packname = getPackageName();
		string sofilename = string("/data/data/") + packname + "/youkuEntry/" + OLDSO_FILE_NAME;
		gOldHandle = (soinfo*)dlopen(sofilename.c_str(),RTLD_LAZY|RTLD_GLOBAL);
		if(gOldHandle <= 0){
			sprintf(szout,"dlopen:%s error\r\n",sofilename.c_str());
			writeLogFile(szout);
			return 0;
		}
	}

	OLDFUNCTION oldfun = (OLDFUNCTION)dlsym(gOldHandle,funname.c_str());
	if(oldfun){
		int ret = oldfun( a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
		return ret;
	}

	__android_log_print(ANDROID_LOG_ERROR,YOUKU_LOG_TAG,"vod so function:%s not found",funname.c_str());

	sprintf(szout,"vod so dlsym:%s error\r\n",funname.c_str());
	writeLogFile(szout);

	return 0;
}

extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_start(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){

	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_start",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_stop(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_stop",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_exit(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_exit",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_PCDNAddress(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_PCDNAddress",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_PCDNSet(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_PCDNSet",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_PCDNSetByKV(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_PCDNSetByKV",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_PCDNSetByHint(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_PCDNSetByHint",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_PCDNGet(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_PCDNGet",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_PCDNGetByKV(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_PCDNGetByKV",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_getVersion(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_getVersion",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
extern "C" JNIEXPORT int JNICALL Java_com_edge_pcdn_PcdnAcc_notifyAPS(int a1,int a2,int a3,int a4,
		int a5,int a6,int a7,int a8,int a9,int a10,int a11,int a12){
	return myoldFunction("Java_com_edge_pcdn_PcdnAcc_notifyAPS",a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
